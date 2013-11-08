"""
  Contains functions to set up the environment on the tombo server to run scripts in the 'simulate' 
  package
  
  @author Tom Close 
"""
import sys
if float(sys.version[0:3]) < 2.7:
    raise Exception("This script requires python version 2.7 or greater")

def init_work_dir(work_dir):
    """
    Copies directories from the project directory to the work directory
    
    @param work_dir: The destination work directory
    """   
    pass