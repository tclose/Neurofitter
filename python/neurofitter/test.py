from libneurofitterml import parse
    
settings = parse('/home/tclose/git/neurofitter/xml/butera.xml')
print settings.to_xml()