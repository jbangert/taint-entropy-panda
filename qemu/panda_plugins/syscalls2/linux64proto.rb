#!/usr/bin/env ruby
dirpath = ARGV[0] || "."

signatures = {}
Dir.glob(dirpath + '/**/*.c').each do |filename|
#  print  filename, '\n'
  next if(filename.include? 'arch' and !filename.include? 'x86')
  file = IO.binread(filename)
  file.scan(/SYSCALL_DEFINE\d+\((.*)\)/).each { |m|
    params = m[0].split(",")
    name = "sys_"+params[0]
    args =  params[1..-1]
    if(args.empty?)
      args = "(void)"
    else
      args="("+args.each_slice(2).map{|s| s.join(' ')}.join(",")+")"
    end
    sig  = "long "+ name + args + ";";
    if(signatures[name] !=nil and signatures[name] != sig)
      print signatures[name], "\n", sig , "\n"
      print "Duplicate sig for ", name, "\n"
      raise ArgumentError.new "Duplicate"
    end
    signatures[name] =sig
    # print name, " ", args , "\n"
    # print name, "\n"
         
  }
end

IO.read(dirpath + "/arch/x86/include/generated/asm/syscalls_64.h").gsub(/\s+/, "").scan(/\((\d*),(\w*),(\w*)\)/).each do |m|
  if(m[1] != m[2])
    raise RuntimeError
  end
  if(signatures.include?(m[1]))
    print "#{m[0]} #{signatures[m[1]]}\n"
  else
    print "#{m[0]} #{m[1]}(  void);\n"
  end

end

