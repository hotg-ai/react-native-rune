#include <rune.hpp>
#include <string>

class Runetime
{
private:
    rune::Runtime *runtime;
    rune::Metadata *inputs;
    rune::InputTensors *input_tensors;

public:
    std::string run()
    {
        rune::Error *error = rune_runtime_predict(runtime);
        if (error)
        {
            const char *msg = rune::rune_error_to_string(error);
            rune::rune_error_free(error);
            return msg;
        }
        // reset input tensors
        rune::rune_input_tensors_free(input_tensors);
        input_tensors = rune_runtime_input_tensors(runtime);

        return getOutput();
    }

    void addInputTensor(int raw_node_id, uint8_t *input, uint32_t length)
    {
        size_t dimensions[3] = {513, 513, 3};
        rune::Tensor *raw_input = rune_input_tensors_insert(
            input_tensors,
            raw_node_id,
            rune::U8,
            dimensions,
            3);
        uint8_t *raw_input_buffer = rune::rune_tensor_buffer(raw_input);
        memcpy(raw_input_buffer, input, length);
    }

    std::string load(rune::Config cfg)
    {
        runtime = NULL;
        rune::Error *error = rune::rune_runtime_load(&cfg, &runtime);
        if (error)
        {
            const char *msg = rune::rune_error_to_string(error);
            rune::rune_error_free(error);
            return msg;
        }
        error = rune::rune_runtime_inputs(runtime, &inputs);
        if (error)
        {
            const char *msg = rune::rune_error_to_string(error);
            rune::rune_error_free(error);
            return msg;
        }
        input_tensors = rune_runtime_input_tensors(runtime);

        return getManifest();
    }

    std::string getOutput()
    {
        rune::OutputTensors *outputs = rune::rune_runtime_output_tensors(runtime);

        std::string output = "[output===";
        const rune::OutputTensor *output_tensor;
        uint32_t output_id;

        printf("Reading outputs:\n");
        while (rune::rune_output_tensors_next(outputs, &output_id, &output_tensor))
        {
            const rune::Tensor *fixed = rune::rune_output_tensor_as_fixed(output_tensor);

            if (!fixed)
            {
                printf("Skipping %d (not a fixed size output tensor)\n", output_id);
                continue;
            }

            rune::ElementType element_type = rune_tensor_element_type(fixed);
            size_t rank = rune::rune_tensor_rank(fixed);
            const size_t *dimensions = rune::rune_tensor_dimensions(fixed);
            // uint8_t output = *(uint8_t *)rune::rune_tensor_buffer_readonly(fixed);
            output.append(std::string(" output_id:") + std::to_string(output_id));
            output.append(std::string(" rank:") + std::to_string(rank));
            output.append(std::string(" element_type:") + std::to_string(element_type));
            for (int i = 0; i < rank; i++)
            {
                output.append(std::to_string(i) + std::string(" dimensions:") + std::to_string(dimensions[i]));
            }

            // output.append(std::string(" dimensions: %s", dimensions));
            //  output.append(std::string("rank: %zu", rank));
            //  if (element_type == rune::F32 && rank == 2 && dimensions[0] == 1 && dimensions[1] == 1)
            //{
            //  float value = *(float *)rune::rune_tensor_buffer_readonly(fixed);
            //  output.append("\tOutput %d = [[%f]]\n", output_id, value);
            // }
        }
        output.append(std::string("]"));
        return output;
    }
    std::string getManifest()
    {
        std::string output = "[";
        for (int i = 0; i < rune::rune_metadata_node_count(inputs); i++)
        {
            if (i > 0)
            {
                output.append(std::string(","));
            }
            const rune::Node *node = rune::rune_metadata_get_node(inputs, i);
            uint32_t id = rune::rune_node_id(node);
            const char *kind = rune::rune_node_kind(node);
            output.append(std::string("{\"kind\":\"") + std::string(kind) + std::string("\",\"id\":") + std::to_string(id) + std::string(",\"args\":{"));
            for (int arg_no = 0; arg_no < rune::rune_node_argument_count(node); arg_no++)
            {
                if (arg_no > 0)
                {
                    output.append(std::string(","));
                }
                const char *name = rune::rune_node_get_argument_name(node, arg_no);
                const char *value = rune::rune_node_get_argument_value(node, arg_no);
                output.append(std::string("\"") + std::string(name) + std::string("\":\"") + std::string(value) + std::string("\""));
            }
            output.append(std::string("}}"));
        }
        output.append(std::string("]"));
        return output;
    }
};