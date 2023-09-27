import concurrent.futures
import subprocess
import os
import shutil

# Step 1: Delete the 'build' directory if it exists.
if os.path.exists('build'):
    shutil.rmtree('build')

# Step 2: Navigate to the directory containing your CMakeLists.txt file and your source files (r.cpp and w.cpp).
# Assuming the script is running from the directory containing the source files

# Step 3: Create a new directory for the build files.
os.makedirs('build')

# Step 4: Navigate into the new 'build' directory.
os.chdir('build')

# Step 5: Run cmake to generate the build files.
subprocess.check_call(['cmake', '..'])

# Step 6: Run make to build the project.
subprocess.check_call(['make'])

# Step 7: Now, you can run the executables.

commands = ['./nm']

# Creating pool of processes
with concurrent.futures.ProcessPoolExecutor() as executor:
    # Running processes in parallel
    results = list(executor.map(subprocess.run, [commands]*1))


# # List of commands to run
# commands = [ 
#     ['./cvwait'], 
#     ['./cvnotify']
# ]

# # Start all subprocesses
# processes = [subprocess.Popen(cmd) for cmd in commands]
