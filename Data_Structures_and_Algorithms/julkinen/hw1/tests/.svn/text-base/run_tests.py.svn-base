#! /usr/bin/env python

import os, sys, string, tempfile, shutil, subprocess, threading, signal
from mako.template import Template



#code under test
code = [sys.argv[1], sys.argv[2]]

test_names = [
   "Compilation:",
   "Compilation without warnings:",
   "Public test:",
   "Empty data structure:",
   "Same birth years",
   "Oldest",
   "Youngest",
   "More personnel",
   "Personnel count",
   "Efficiency:"
]

test_results = [
   "Compile was success",
   "0/100",
   "0/100",
   "0/100",
   "0/100",
   "0/100",
   "0/100",
   "0/100",
   "0/100",
   "0/100"   
]

test_average = 0

sut = tempfile.mkdtemp()+"/"

home = "/home/tie20106/excercises_public/hw1_redshirt/"
julkinen = "./"

def main():    
   global test_average
   student = os.getcwd()+"/"
   
   if code[0][:1] == '/':
      hdr = code[0]
   else:
      hdr = student+code[0]
   if code[1][:1] == '/':
      src = code[1]
   else:
      src = student+code[1]
   
   
   tests_in = [
      julkinen+"test_public.in",
      julkinen+"test_empty_ds.in",
      julkinen+"test_same_birth_years.in",
      julkinen+"test_oldest.in",
      julkinen+"test_youngest.in",
      julkinen+"test_more_personnel.in",
      julkinen+"test_personnel_count.in"
   ]

   tests_out =[
      julkinen+"test_public.out",
      julkinen+"test_empty_ds.out",
      julkinen+"test_same_birth_years.out",
      julkinen+"test_oldest.out",
      julkinen+"test_youngest.out",
      julkinen+"test_more_personnel.out",
      julkinen+"test_personnel_count.out"
   ]
    
   graphs_in = julkinen + "test_graphs.in"
   eff_in = julkinen+"test_eff.in"

   #starting the testing process
   shutil.copy(home+"Makefile_tiraka", sut + 'Makefile')
   shutil.copy(home+"main.cc", sut)
   shutil.copy(hdr, sut)
   shutil.copy(src, sut)

   points = []
   retval = compile(sut)
   #if the program did not compile we will return
   if retval != 0:
      test_results[0] = "Compile failed"
      cleanup(sut)
      renderTemplate(0)
      return 1
   else:
      warnings = checkWarnings(sut)
      test_results[1] = str(warnings) + "/100"
      test_average += warnings

   printLine()
   print "            Running tests."
   print "This may take few minutes, please wait."
   printLine()

   #running tests
   diffTest(tests_in, tests_out, sut, home)
   
   # +1 for warnings
   average = test_average /  (len(tests_in) + 1)
   
   graphTest(graphs_in, sut, julkinen)
   eff_points = effTest(eff_in, sut, home)
   test_results[9] = str(eff_points) + "/100"

   
   #Printing the final results of the tests
   renderTemplate(average)
   cleanup(sut)

   #kill remaining tests if they timeouted
   print "Killing remaining tests if they timeouted"
   subprocess.Popen(['killall', 'tirakaeff'])
   subprocess.Popen(['killall', 'tirakaprog'])
   return 0

def renderTemplate(average):
   
   printLine()
   results = zip(test_names, test_results)
   
   tmplData = {}
   for i, (name, result) in enumerate(results):
      tmplData["Test" + str(i) + "Name"] = name
      tmplData["Test" + str(i) + "Result"] = result
   
   tmplData['TestAverage'] = 'Test Average'
   tmplData['TestAverageResult'] = str(average) + "/100"

   mytemplate = Template(filename=julkinen + "template.html")
   resultFile = open("../results/index.html", 'w')
   resultFile.write(mytemplate.render(**tmplData));
   subprocess.Popen(['mv', "find.png", "add.png", "del.png", "old.png", "young.png", "../results/"])
   subprocess.Popen(['cp', "template.css", "../results/"])
   #subprocess.Popen(['rm', "find.dat", "add.dat", "del.dat", "old.dat", "young.dat" ])

   printScore(test_results, average)
   print "Testing done, results are available in:"
   print "julkinen/hw1/results"
   
   printLine()
   
   print "To view results in browser,"
   print "you can copy the results directory to your public_html"
   print "with following commands:"
   
   print "If the directory doesn't exist:"
   print "mkdir ~/public_html"
   print "chmod o+rx ~/public_html"
   print ""

   print "cp -r ../results ~/public_html/"
   print "chmod o+rx ~/public_html/results"
   print "chmod -R o+r ~/public_html/results/"
   
   printLine()
   
   print "Then results are viewable in:"
   print "http://www.cs.tut.fi/~username/results"
   printLine()

def printLine():
   print "------------------------------------------"

def compile(sut_):
   warnings = open(sut_+"warnings", 'w')
   p = subprocess.Popen(["make", "-C", sut_], stderr=warnings)
   retval = p.wait()
   return retval

def checkWarnings(sut_):
   sum = 0
   with open(sut_+"warnings", 'r') as file:
      for line in file:
         if "warnings generated" in line:
            t = line.split()
            t = int(t[0])
            #if there is more than 20 warnings the score will be 0
            if t > 20:
               return 0
            else:
               sum = sum + t
   if sum > 20:
      return 0
   elif sum == 0:
      return 100
   else:
      score = int(100-((sum*100)/20))
      return score


def printScore(points, average):
   score = 0
   tests = 0
   printLine()
   for i in range(len(test_names)):

      if points[i] is not False:
         print "  "+test_names[i]+" "+points[i]
         #score = score + points[i]
         #tests = tests+1
      else:
         print "  "+test_names[i]+" Discarded from the final score."

   #score = score/tests
   printLine()
   print "Final score: "+str(average)+" / 100"
   printLine()

#functional tests
def diffTest(inputs, outputs, sut_, home):
   global test_average   
   for i in range(len(inputs)):
      command = sut_+"tirakaprog"
      out_file = sut_+"test"+str(i)+".out"
      test = Test(command, inputs[i], out_file, False)
      timeouted = test.run(timeout=30)
      
      if timeouted == -15:
         print test_names[i+2]+"Test aborted due to timeout."
         print "Continuing tests, please wait."
         test_results[i + 2] = str(0) + "/100"
      elif timeouted != 0:
         print test_names[i+2]+"Test aborted due to runtime error."
         print "Continuing tests, please wait."
         test_results[i + 2] = str(0) + "/100"
      else:
         diff_file = sut_+"diff"+str(i)
         diff = open(diff_file, 'w')
         p = subprocess.Popen(["diff","-y","--suppress-common-lines",outputs[i],out_file], stdout = diff)
         p.wait()
         diff.flush()

         wc = subprocess.Popen(["wc", "-l", diff_file], stdout=subprocess.PIPE)
         tmp = wc.stdout.read()
         diff_lines = [int(s) for s in tmp.split() if s.isdigit()]
         wc = subprocess.Popen(["wc", "-l", outputs[i]], stdout=subprocess.PIPE)
         tmp = wc.stdout.read()
         output_lines = [int(s) for s in tmp.split() if s.isdigit()]
         difference = 100-(float(diff_lines[0])*100 / float(output_lines[0]))
         
         if difference < 0:
            test_results[i + 2] = str(0) + "/100"
         else:
            test_results[i + 2] = str(int(difference)) + "/100"
            test_average += int(difference)
        
    
def graphTest(input, sut_, julkinen):
   sut_eff = sut_+"tirakaeff"
   out_file = "/dev/null"
   time_sut = sut_+"time"
   test = Test(sut_eff, input, out_file, time_sut)
   
   timeouted = test.run(timeout=420)
   if timeouted == -15:
      print "Graph generation aborted due to timeout."
   DEVNULL = open(os.devnull, 'wb')
   subprocess.Popen(['gnuplot', julkinen + "plot.gpl"], stdout=DEVNULL, stderr=DEVNULL)
      
   return 0

   
def effTest(input, sut_, home):
   model_eff = home+"tirakaeff"
   sut_eff = sut_+"tirakaeff"
   out_file = sut_+"effout"

   sum_model = 0
   sum_sut = 0

   measurements = 7

   for i in range(measurements):
      time_sut = sut_+"time"+str(i)
      test = Test(sut_eff, input, out_file, time_sut)
      timeouted = test.run(timeout=30)
      if timeouted == -15:
         print "Efficiency: test aborted due to timeout."
         print "Continuing tests, please wait."
         return 0
      elif timeouted != 0:
         print "Efficiency: test aborted due to runtime error."
         print "Continuing tests, please wait."
         return 0
      time_model = sut_+"timeM"+str(i)
      test = Test(model_eff, input, out_file, time_model)
      timeouted = test.run(timeout=30)
      if timeouted != 0:
         printLine()
         print "Could not finish efficiency test due to problems in model program."
         print "Test will be discarded from the final score"
         print "If you get this error please send an email to course staff"
         print "and try to run the script again later."
         printLine()
         return False

      sum_sut = sum_sut + parseTime(time_sut)
      sum_model = sum_model + parseTime(time_model)

   avg_model = sum_model/measurements
   avg_sut = sum_sut/measurements
   #if the tested program was faster than model we know that score is 100
   #else we can determine it by calculating how much faster the model was
   if avg_sut <= avg_model:
      score = 100
   else:
      score = int((avg_model/avg_sut)*100)
   return score

#helper function for parsing file that contains output from
#bash time
def parseTime(input):
   with open(input, 'r') as file:
      for line in file:
         if "user" in line:
            #splitting "sys" and "0mS.MMMs" to two variables in array
            t = line.split()
            #converting the 0mS.MMMs" to format "S.MMM" and to float
            t = float(t[1][2:-1])
            return t

def exit_cleanup(signum, frame):
   signal.signal(signal.SIGINT, original_sigint)
   cleanup(sut)
   sys.exit(1)

#removes the temporary directory
def cleanup(sut_):
   shutil.rmtree(sut_)

#test object which provides timeout
class Test(object):
   def __init__(self, command, in_file, out_file, err_file):
      self.command = command
      self.in_file = in_file
      self.out_file = out_file
      #err file is defined only when doing eff test
      self.err_file = err_file
      self.process = None
    
   def run(self, timeout):
      def target():
         input = open(self.in_file)
         output = open(self.out_file, 'w')
         if self.err_file:
            err = open(self.err_file, 'w')
            self.process = \
               subprocess.Popen("time "+self.command, stdin=input, stdout=output, stderr=err, shell=True)
         else:
            self.process = \
               subprocess.Popen(self.command, stdin=input, stdout=output)
         self.process.wait()
         output.flush()
         if self.err_file:
            err.flush()

      thread = threading.Thread(target=target)
      thread.start()
      thread.join(timeout)
      if thread.is_alive():
         self.process.terminate()
         thread.join()

      return self.process.returncode


#starting everything...
if __name__ == '__main__':
   # store the original SIGINT handler
   original_sigint = signal.getsignal(signal.SIGINT)
   signal.signal(signal.SIGINT, exit_cleanup)
   main()
