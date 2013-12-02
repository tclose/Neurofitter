import shutil
import os
from neurofitter import Settings

s = Settings.load('/home/tclose/git/neurofitter/xml/roimpisettings.xml')
try:
    shutil.rmtree('/home/tclose/Desktop/neurofitter-test/output')
except OSError:
    pass
os.mkdir('/home/tclose/Desktop/neurofitter-test/output')
s.set_work_directory('/home/tclose/Desktop/neurofitter-test/output')
print s.experiment.files_list           
