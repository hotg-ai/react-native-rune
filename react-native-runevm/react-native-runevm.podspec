# react-native-runevm.podspec

Pod::Spec.new do |s|
  s.name         = "react-native-runevm"
  s.version      = "1.0.0"
  s.summary          = 'RuneVM for React Native plugin'
  s.description      = <<-DESC
  RuneVM for RN plugin
                       DESC
  s.homepage         = 'http://hotg.ai'
  s.license          = { :file => 'LICENSE' }
  s.author           = { 'Hammer Of The Gods' => 'admin@hotg.ai' }
  s.source           = { :path => '.' }
  s.source_files = 'ios/Classes/**/*'
  s.platform = :ios, '12.1'
  s.public_header_files = 'ios/Classes/**/*.h'
  s.xcconfig = { 
    'LIBRARY_SEARCH_PATHS' => '"${PODS_TARGET_SRCROOT}/ios/install-ios-rel/lib/"', 
    'HEADER_SEARCH_PATHS' => '"${PODS_TARGET_SRCROOT}/ios/install-ios-rel/include/" "${PODS_TARGET_SRCROOT}/ios/Classes/"',
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'CLANG_CXX_LIBRARY' => 'libc++'
  }
  # Flutter.framework does not contain a i386 slice.
  s.vendored_libraries = 'ios/install-ios-rel/lib/*.a'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.source_files = "ios/**/*.{h,c,cc,cpp,m,mm,swift}"
  s.requires_arc = true

  s.dependency "React"
  # ...
  # s.dependency "..."
end

