import sys
import os
import platform
import types
import subprocess
from optparse import OptionParser
import shutil

cmake_android_arch_dict = {
    'arm':'-DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a -DCMAKE_ANDROID_ARM_NEON=on',
    'arm64':'-DCMAKE_ANDROID_ARCH_ABI=arm64-v8a',
    'x86_64':'-DCMAKE_ANDROID_ARCH_ABI=x86_64',
    'x86':'-DCMAKE_ANDROID_ARCH_ABI=x86'
}

cmake_windows_gen_dict = {
    'vs2017':'Visual Studio 15 2017',
    'vs2015':'Visual Studio 14 2015',
    'vs2013':'Visual Studio 12 2013',
    'vs2012':'Visual Studio 11 2012'
}

def public_flag(build_test, build_tools, with_simd):
    command = ' -DSKULD_WITH_SIMD=on' if with_simd else ' -DSKULD_WITH_SIMD=off'
    if (build_test):
        command = command + ' -DSKULD_BUILD_TESTS=on'
    if (build_tools):
        command = command + ' -DSKULD_BUILD_TOOLS=on'
    return command
	
def BuildAndroid(ndk_path, arch, build_test):
    build_dir = 'build/build-android-' + arch
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)
    os.chdir(build_dir)
    command = ('cmake ../.. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Android ' +
               '-DCMAKE_SYSTEM_VERSION=24 -DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang -DCMAKE_ANDROID_NDK="' + ndk_path.replace("\\", "/") + '" ' +
               cmake_android_arch_dict[arch])
    if ('Windows' in platform.system()):
        ANDROID_NDK_MAKE = '/prebuilt/windows/bin/make.exe'
        if (not os.path.exists(ndk_path + ANDROID_NDK_MAKE)):
            ANDROID_NDK_MAKE = '/prebuilt/windows-x86_64/bin/make.exe'
        command = command + ' -DCMAKE_MAKE_PROGRAM="'+ ndk_path + ANDROID_NDK_MAKE + '" '
    command = command + public_flag(build_test, False, True)
    os.system(command)
    os.system('cmake --build . --config Release')
    os.chdir('../..')

def BuildWindows(arch, __tool, build_test, build_tools):
    build_dir = 'build/build-windows-' + __tool + '-' + arch
    tool = cmake_windows_gen_dict[__tool]
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)
    os.chdir(build_dir)
    if (arch == 'x86_64'):
        _tool = tool + ' Win64'
    elif (arch == 'arm'):
        _tool = tool + ' ARM'
    else:
        _tool = tool
    command = ('cmake ../.. ' +
               '-G"' + _tool + '" ' +
               public_flag(build_test, build_tools, True))
    os.system(command)
    os.system('cmake --build . --config Release')
    os.chdir('../..')

def BuildUnix(arch, build_test, build_tools):
    build_dir = 'build/build-unix-' + arch
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)
    os.chdir(build_dir)
    command = ('cmake ../.. -G"Unix Makefiles" ' +
               public_flag(build_test, build_tools, True))
    if arch == 'x86':
        command = command + ' -DUNIX_BUILD_32=on'
    elif arch == 'x86_64':
        command = command + ' -DUNIX_BUILD_64=on'
    os.system(command)
    os.system('cmake --build . --config Release')
    os.chdir('../..')

def BuildMac(build_test, build_tools):
    build_dir = 'build/build-mac-x86_64'
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)
    os.chdir(build_dir)
    command = ('cmake ../.. -G"Unix Makefiles" ' +
               public_flag(build_test, build_tools, True))
    os.system(command)
    os.system('cmake --build . --config Release')
    os.chdir('../..')

def Build(args):
    parser = OptionParser()
    parser.add_option('-p', '--platform', dest='platform', default='Windows', help='Dest platform(Windows, Linux, Android, iOS, macOS, UWP, WindowsStore, WindowsPhone)')
    parser.add_option('-a', '--arch', dest='arch', default='x86_64', help='Dest architecture(x86_64, x86, arm, arm64)')
    parser.add_option('-x', '--test', dest='build_test', action='store_true', default=False, help='Build tests')
    parser.add_option('-o', '--tool', dest='build_tool', action='store_true', default=False, help='Build tools(Only on Windows, Linux and macOS)')
    parser.add_option('-t', '--windows-build-tool', dest='wintool', default='vs2017', help='Tool to build on Windows(vs2017, vs2015, vs2013, vs2012)')
    parser.add_option('-n', '--ndk', dest='ndk_path', type='string', help='Android NDK Path, Default to env var ANDROID_NDK(will be used when build for android)')
    (options, _args) = parser.parse_args(args)
    arch_list = ['x86_64', 'x86', 'arm', 'arm64']
    if (not os.path.exists('build')):
        os.mkdir('build')
    if (not options.arch in arch_list):
        print ("Error: Unknown or unsupported arch")
        sys.exit()
    print ('Build for ' + options.platform + '-' + options.arch)
    if (options.platform == 'Android'):
        if ((options.ndk_path is None) or (not os.path.exists(os.path.abspath(options.ndk_path + "/ndk-build.cmd")))):
            print ("Error: Cannot find Android NDK")
            sys.exit();
        else:
            ndk = options.ndk_path
        BuildAndroid(ndk, options.arch, options.build_test)
    elif(options.platform == 'Windows'):
        if (not 'Windows' in platform.system()):
            print ("Error: Please build on Windows")
            sys.exit();
        if (not options.wintool in cmake_windows_gen_dict):
            print ('Error: Unsupported tool')
            sys.exit();
        if (options.arch == 'arm64'):
            print ('Error: Unsupported arch')
            sys.exit();
        BuildWindows(options.arch, options.wintool, options.build_test, options.build_tool)
    elif(options.platform == 'Linux'):
        if (not 'Linux' in platform.system()):
            print ("Error: Please build on Linux")
            sys.exit();
        if (options.arch == 'arm' or options.arch == 'arm64'):
            print ('Error: It doesn\'t support arm or arm64 for unix now.')
            sys.exit();
        BuildUnix(options.arch, options.build_test, options.build_tool);
    elif(options.platform == 'macOS'):
        if(not 'Darwin' in platform.system()):
            print ('Error: Please build on macOS')
            sys.exit()
        if (options.arch == 'arm' or options.arch == 'x86' or options.arch == 'arm64'):
            print ('Error: It doesn\'t support arm, arm64 or x86 for macOS.')
            sys.exit()
        BuildMac(options.build_test, options.build_tool)
    elif(options.platform == 'iOS'):
        if(not 'Darwin' in platform.system()):
            print ('Error: Please build on macOS')
            sys.exit()
    elif(options.platform == 'UWP'):
        print ('Stopped')
        sys.exit()
    elif(options.platform == 'WindowsStore'):
        print ('Stopped')
        sys.exit()
    elif(options.platform == 'WindowsPhone'):
        print ('Stopped')
        sys.exit()
    else:
        print ("Error: Unknown or unsupported platform")
        sys.exit()

def AutoBuild(argv):
    parser = OptionParser()
    parser.add_option('-p', '--platform', dest='platform', default='Windows', help='Dest platform(Windows, Linux, Android, iOS, macOS, UWP, WindowsStore, WindowsPhone)')
    parser.add_option('-t', '--windows-build-tool', dest='wintool', default='vs2017', help='Tool to build on Windows(vs2017, vs2015, vs2013, vs2012)')
    parser.add_option('-n', '--ndk', dest='ndk_path', type='string', help='Android NDK Path, Default to env var ANDROID_NDK(will be used when build for android)')
    (options, _args) = parser.parse_args(argv)
    if options.platform == 'Android':
        option_list = [
            ['-p', 'Android', '-x', '-a', 'arm'],
            ['-p', 'Android', '-x', '-a', 'arm64'],
            ['-p', 'Android', '-x', '-a', 'x86'],
            ['-p', 'Android', '-x', '-a', 'x86_64']
        ]
        if not (options.ndk_path is None):
            for opt in option_list:
                opt.append('-n')
                opt.append(options.ndk_path)
        for opt in option_list:
            Build(opt)
    elif options.platform == 'Windows':
        option_list = [
            ['-p', 'Windows', '-x', '-a', 'arm'],
            ['-p', 'Windows', '-x', '-a', 'x86', '-o'],
            ['-p', 'Windows', '-x', '-a', 'x86_64', '-o']
        ]
        if not (options.wintool is None):
            for opt in option_list:
                opt.append('-t')
                opt.append(options.wintool)
        for opt in option_list:
            Build(opt)
    elif options.platform == 'macOS':
        option_list = [
            ['-p', 'macOS', '-x', '-a', 'x86_64', '-o']
        ]
        for opt in option_list:
            Build(opt)
    elif options.platform == 'Linux':
        option_list = [
            ['-p', 'Linux', '-x', '-a', 'x86', '-o'],
            ['-p', 'Linux', '-x', '-a', 'x86_64', '-o']
        ]
        for opt in option_list:
            Build(opt)
    else:
        print ("Error: Unknown platform.")

def CopyFile(src, des):
    if (os.path.exists(src)):
        if not os.path.exists(os.path.split(des)[0]):
            os.makedirs(os.path.split(des)[0])
        with open(des, "wb") as f1:
            with open(src, "rb") as f2:
                f1.write(f2.read()) 

def Deploy(_path):
    core_names = ['Core']
    plugin_names = [
        'RenderPlugin3DD3D11', 
        'RenderPlugin3DVulkan',
        'RenderPlugin3DOpenGL', 
        'RenderPlugin3DOpenGLES',
        'RenderPlugin3DMetal',
        'RenderPlugin2DD2D',
        'AudioPluginXAudio2',
        'AudioPluginXAudio2_7',
        'AudioPluginOpenSLES',
        'AudioPluginOpenAL'
    ]
    tool_names = [
        'ModelConverter',
        'vprojc',
        'EffectCompiler'
    ]
    path = os.path.abspath(_path)
    os.chdir('build')
    for filename in os.listdir('.'):
        fp = os.path.join('.', filename)
        if os.path.isdir(fp) and (not filename == 'deploy'): #-----
            os.chdir(filename)
            tokens = filename.split('-')
            if (tokens[1] == 'windows'):
                for plugin in plugin_names:
                    CopyFile('./bin/Release/' + plugin + '.dll', path + './libs/Windows/' + tokens[3] + '/' + plugin + '.dll')
                    CopyFile('./bin/Release/' + plugin + 'Static.lib', path + './libs/Windows/' + tokens[3] + '/' + plugin + 'Static.lib')
                for core in core_names:
                    CopyFile('./bin/Release/' + core + '.dll', path + './libs/Windows/' + tokens[3] + '/' + core + '.dll')
                    CopyFile('./bin/Release/' + core + '.lib', path + './libs/Windows/' + tokens[3] + '/' + core + '.lib')
                    CopyFile('./bin/Release/' + core + 'Static.lib', path + './libs/Windows/' + tokens[3] + '/' + core + 'Static.lib')
                for tool in tool_names:
                    CopyFile('./bin/Release/' + tool + '.exe', path + './tools/Windows/' + tokens[3] + '/' + tool + '.exe')
            elif (tokens[1] == 'mac'):
                for plugin in plugin_names:
                    CopyFile('./bin/lib' + plugin + '.dylib', path + './libs/macOS/' + tokens[2] + '/lib' + plugin + '.dylib')
                    CopyFile('./bin/lib' + plugin + 'Static.a', path + './libs/macOS/' + tokens[2] + '/lib' + plugin + 'Static.a')
                for core in core_names:
                    CopyFile('./bin/lib' + core + '.dylib', path + './libs/macOS/' + tokens[2] + '/lib' + core + '.dylib')
                    CopyFile('./bin/lib' + core + 'Static.a', path + './libs/macOS/' + tokens[2] + '/lib' + core + 'Static.a')
                for tool in tool_names:
                    CopyFile('./bin/' + tool, path + './tools/macOS/' + tokens[2] + '/' + tool)
            elif (tokens[1] == 'unix'):
                for plugin in plugin_names:
                    CopyFile('./bin/lib' + plugin + '.so', path + './libs/Linux/' + tokens[2] + '/lib' + plugin + '.so')
                    CopyFile('./bin/lib' + plugin + 'Static.a', path + './libs/Linux/' + tokens[2] + '/lib' + plugin + 'Static.a')
                for core in core_names:
                    CopyFile('./bin/lib' + core + '.so', path + './libs/Linux/' + tokens[2] + '/lib' + core + '.so')
                    CopyFile('./bin/lib' + core + 'Static.a', path + './libs/Linux/' + tokens[2] + '/lib' + core + 'Static.a')
                for tool in tool_names:
                    CopyFile('./bin/' + tool, path + './tools/Linux/' + tokens[2] + '/' + tool)
            elif (tokens[1] == 'android'):
                arch = {
                    'x86':'x86',
                    'x86_64':'x86_64',
                    'arm':'armeabi-v7a',
                    'arm64':'arm64-v8a'
                }
                for plugin in plugin_names:
                    CopyFile('./bin/lib' + plugin + '.so', path + './libs/Android/' + arch[tokens[2]] + '/lib' + plugin + '.so')
                    CopyFile('./bin/lib' + plugin + 'Static.a', path + './libs/Android/' + arch[tokens[2]] + '/lib' + plugin + 'Static.a')
                for core in core_names:
                    CopyFile('./bin/lib' + core + '.so', path + './libs/Android/' + arch[tokens[2]] + '/lib' + core + '.so')
                    CopyFile('./bin/lib' + core + 'Static.a', path + './libs/Android/' + arch[tokens[2]] + '/lib' + core + 'Static.a')
            os.chdir('../')
    os.chdir('../')

if __name__ == '__main__':
    if (len(sys.argv) < 2):
        print ('Select action: build, deploy, clean, autobuild')
        sys.exit()
    if (sys.argv[1] == 'build'):
        Build(sys.argv)
    elif (sys.argv[1] == 'clean'):
        shutil.rmtree('./build')
    elif (sys.argv[1] == 'autobuild'):
        AutoBuild(sys.argv)
    elif (sys.argv[1] == 'deploy'):
        if (len(sys.argv) < 3):
            Deploy('./build/deploy/')
        else:
            Deploy(sys.argv[2])
    else:
        print ('Select action: prebuild, build, deploy, clean, autobuild')

#---