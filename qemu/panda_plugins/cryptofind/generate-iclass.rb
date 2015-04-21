#!/usr/bin/ruby
#https://robm.me.uk/ruby/2014/01/06/nokogiri-command-line.html

require "nokogiri"
require "open-uri"
tablename = ARGV.shift or "opcode_class"



doc = Nokogiri::HTML(ARGF)

def icls(opcode, cls)
  if opcode.include? "MOV"
    return "ICLS_MOV"
  end
  if ["logical", "shftrot", "bit", "binary", "arith"].include? cls
    return "ICLS_BITARITH"
  end
  return "ICLS_NORMAL"
end
rows = doc.css("table.ref_table tr")
print "#include \"iclass.h\"\n"
print "#include <distorm.h>\n"
print "#include <mnemonics.h>\n"
print "char #{tablename}[65536];\n"
print "void init_#{tablename}(){"
print "memset(&#{tablename},ICLS_WARN, sizeof #{tablename});\n" 
rows.each do |tr|
  text = tr.css('td').map(&:text)
  opcode = text[11]
  cls = text[17]
  next unless cls.is_a? String and opcode.is_a? String
  next if opcode == "" or opcode == "invalid" or opcode == "null" or opcode =="undefined"
  next if opcode.include? "alias"

  out = icls(opcode,cls)
  print("\t#{tablename}[I_#{opcode}]= #{out};\n")
end


print "}\n"
