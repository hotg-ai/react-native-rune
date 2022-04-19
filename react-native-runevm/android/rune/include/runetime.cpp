#include <rune.hpp>
#include <string>

class Runetime {
    private:       
        rune::Runtime *runtime;
        rune::Metadata *inputs; 
    public:             
        const char* run(rune::Config cfg) {

        }

        const char* load(rune::Config cfg) {
            runtime = NULL;
            rune::Error *error = rune::rune_runtime_load(&cfg, &runtime);
            if (error) {
                const char * msg = rune::rune_error_to_string(error);
                rune::rune_error_free(error);
                return msg;
            }
            error = rune::rune_runtime_inputs(runtime, &inputs);
            if (error) {
                const char * msg = rune::rune_error_to_string(error);
                rune::rune_error_free(error);
                return msg;
            }
            InputTensors *input_tensors = rune::rune_runtime_input_tensors(runtime);

            return getManifest();
        }   

        const char* getManifest() {
            std::string output = "[";
            for(int i = 0; i < rune::rune_metadata_node_count(inputs); i++) {
                if(i>0) {
                    output.append(std::string(","));
                }
                const rune::Node *node = rune::rune_metadata_get_node(inputs, i);
                uint32_t id = rune::rune_node_id(node);
                const char *kind = rune::rune_node_kind(node);
                output.append(std::string("{\"kind\":\"") + std::string(kind) + std::string("\",\"id\":") + std::to_string(id)+std::string(",\"args\":{"));
                for(int arg_no = 0; arg_no < rune::rune_node_argument_count(node); arg_no++) {
                    if(arg_no>0) {
                        output.append(std::string(","));
                    }
                    const char *name = rune::rune_node_get_argument_name(node, arg_no);
                    const char *value = rune::rune_node_get_argument_value(node, arg_no);
                    output.append(std::string("\"") + std::string(name) + std::string("\":\"") + std::string(value) + std::string("\""));
                }
                output.append(std::string("}}"));
            }
            output.append(std::string("]"));
            char const *c_result = output.data();
            return c_result;
        }   
};