# Chad Miller March 2014
# gcc has form
# int vfprintf(FILE *stream, const char *format, va_list arg)
# musl has form
# int musl_vfprintf(MUSL_FILE *restrict f, const char *restrict fmt, va_list ap)
# grammar has format: %[flags][width][.precision][length]specifier 
class TestPrintf 

  def intialize
    @includes
    @main
    @main_code
    @code
    @type = ""
    @fmt = ""
    @args = ""
    @snprintf 
    @stream
    @specifier = {'c'=>'c','d'=>'d','e'=>'e','E'=>'E',
      'f'=>'f','g'=>'g','G'=>'G','o'=>'o','s'=>'s',
      'u'=>'u', 'x'=>'x','p'=>'p','n'=>'n'}
    @flags = {'-'=>'-','+'=>'+','#'=>'#','0'=>'0'}
    @width = {'w'=>"#{w}",'arg'=>'*'}
    @w #number
    @precision = {'p'=>".#{p}", 'arg'=>'.*'}
    @p #number
    leng = {'h'=>'h','l'=>'l','L'=>'L'}
    @test_musl = ""
    @test_gcc = ""
  end

  # Run it and test it
  def run_fuzz

    write_fuzz
    mk_muslf
    mk_gccf
    test_it

  end

  # Write the actual fuzzer
  def write_fuzz
    write_musl
  end

  # Makes a musl test file
  def mk_muslf
    @includes = "#include <stdio.h>\n#include <stdarg.h>\n#include \"musl.h\"\n"\
    "#define BUFF_SIZE 100\n\n"
    @type = "musl_"
    @main_code = " char buff[BUFF_SIZE];\n  #{@test_musl}\n  fputs(buff, stdout);\n  return 0;"
    @main = "\nint main(){\n\n #{@main_code}\n}\n"
    file = File.open("test-musl.c", 'w') 
    @code = @includes+@main
    file.write(@code) 
    file.close
  end

  # Makes a gcc test file (for differential testing)
  def mk_gccf
    @includes = "#include <stdio.h>\n#include <stdarg.h>\n#define BUFF_SIZE 100\n"
    @type = ""
    @main_code = " char buff[BUFF_SIZE];\n"\
                 "  #{@test_gcc}\n  fputs(buff, stdout);\n  return 0;"
    @main = "\nint main(){\n\n #{@main_code}\n}\n"
    @code = @includes+@main
    file = File.open("test-gcc.c", 'w') 
    file.write(@code) 
    file.close
  end

  # Differential testing
  def test_it
    compile_and_exec
  end

  # Compile two file, exec and compare them
  def compile_and_exec
    system("gcc -g -o gcc_exec test-gcc.c")
    system("gcc -g -ftest-coverage -fprofile-arcs -o musl_exec"\
           " test-musl.c snprintf.c vsnprintf.c vfprintf.c fwrite.c")
    s1 = %x[./gcc_exec]
    s2 = %x[./musl_exec]
    puts s1 == s2
    #stdout_gcc = IO.popen("./gcc_exec")
    #stdout_musl = IO.popen("./musl_exec")
    #%x(diff stdout_gcc stdout_musl)
  end


  # Write musl fuzzer
  def write_musl

    @fmt = "\"%s\""
    @args ="\"hello\""
    @type = "musl_"
    @snprintf = "#{@type}snprintf(buff, BUFF_SIZE, #{@fmt},#{@args});\n"
    @test_musl = @snprintf
  end


end


tpf = TestPrintf.new
tpf.run_fuzz
