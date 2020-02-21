# MAKE.py
#   by Lut99
#
# Created:
#   21/02/2020, 20:33:09
# Last edited:
#   21/02/2020, 20:33:09
# Auto updated?
#   Yes
#
# Description:
#   This file contains the custom, cross-platform Makefile. By using Python
#   as the scripting language, we hope to avoid having to use make and such.
#

import argparse
import platform
import os


### DEFINE GLOBALS ###

# Define the general source and bin folders
BIN = "bin"
SRC = "src"

# Define library folders
LIB_SRC = SRC + "/lib"
LIB_BIN = BIN + "/bin"
ARCHIVES = LIB_BIN + "/archives"

# Define tests
TESTS_SRC = "tests/src"
TESTS_BIN = "tests/bin"

# Define what folders need to be cleaned (this is all .out, .o and .a files)
TO_CLEAN = (BIN, TESTS_BIN)
TO_CLEAN_EXT = ("out", "exe", "a", "o")

# Define compilers
UNIX_CC = "g++"
WIN_CC = ""

# Define compiler flags
UNIX_CC_ARGS = "-std=c++17 -O2 -Wall -Wextra"
WIN_CC_ARGS = ""



class Actions:
    """
        This class is the collection of possible actions. Simply add one here
        to automatically add it to the script. Note that the action must be
        prefixed with "ACTION_".
    """

    def __init__(self):
        # Collect the actions
        self._actions = {}
        for name in dir(self):
            if callable(getattr(self, name)) and name[:7] == "ACTION_":
                self._actions[name[7:].lower()] = name

    def __contains__(self, elem):
        """ Returns true if given elem occurs in the action list """
        if type(elem) != str:
            return False
        return elem.lower() in self._actions

    def build(self, action, os, threaded):
        """
            Builds given action. If action does not exist, displays an error
            message. The os should be one of the implemented OSes, but no
            checking is done for that. Finally threaded is a boolean indicating
            if it should compiled with threading support.
        """

        if action not in self:
            print(f"  ERROR: Unknown build target '{action}'")
            return
        
        # Run the function
        return getattr(self, self._actions[action])(os, threaded)

    def ACTION_Raytracer(self, os, raytracer):
        """
            Builds the main raytracer executable. To ease up life, it compiles
            everything it can find in the LIB_SRC dir and that ends in .cpp.
            To create and use archive files, specify them in the ARCHIVES
        """
        print("Building raytracer...")

        print("  Building libraries...")

        print("Done")



def clean():
    """
        Cleans the folders that are specified above
    """

    print("Running clean operation...")

    to_do = list(TO_CLEAN)
    while len(to_do) > 0:
        elem = to_do.pop()

        if os.path.isdir(elem):
            # List the target files and add them to the to_do list (depth first)
            files = os.listdir(elem)
            for file in files:
                _, ext = os.path.splitext(file)
                if ext in TO_CLEAN_EXT:
                    elem.append(elem + "/" + file)
        elif os.path.isfile(elem):
            # Simply remove it
            os.remove(elem)
            print(f"  Cleaned {elem}")
        else:
            print("  FAILURE: not found")
    
    print("Done")



if __name__ == "__main__":
    # Declare the action variable with s default value
    action = "raytracer"

    # Initialize the actions
    actions = Actions()

    # Parse the arguments first
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--threaded", help="Compiles the raytracer using threads (both UNIX and Windows systems)", action="store_true")
    parser.add_argument("-c", "--clean", help="If specified, cleans the output directory before compiling", action="store_true")
    parser.add_argument("-u", "--unix", help="Forces UNIX build. Note that this does required (an alias of) g++ to be installed.", action="store_true")
    parser.add_argument("-w", "--win", help="Forces Windows build. Note that this required (an alias of) <> to be installed.", action="store_true")
    parser.add_argument("action", help="Specifies the action of the build. If omitted, builds the raytracer application.", nargs='?')

    args = parser.parse_args()
    if args.action:
        action = args.action

    # Detect the OS
    os = platform.system()
    if os != "Windows" and os != "Linux":
        print(f"ERROR: Unsupported OS '{os}'\n")
        exit()

    # Override if given
    if args.win and args.unix:
        print("ERROR: Cannot specify both --win and --unix\n")
        exit(-1)
    elif args.unix:
        os = "Linux"
    elif args.win:
        os = "Windows"

    print("\n*** BUILD SCRIPT FOR RAYTRACER ***\n")

    # If given, run clean instead
    if action == "clean":
        clean()
        print("\nDone.\n")
        exit()

    # Check if the action is valid
    if action not in actions:
        print(f"ERROR: Unknown build target '{action}'\n")
        exit(-1)

    # It is, let's print some build info
    print(f"Build target: {action}")

    print(f"  Building for {os}")

    print("Using options:", end="")
    if args.threaded: print(" --threaded", end="")
    if args.clean: print(" --clean", end="")
    if args.unix: print(" --unix", end="")
    if args.win: print(" --win", end="")
    if not args.threaded and not args.clean and not args.unix and not args.win:
        print(" -")
    print("")

    print("Folders:")
    print("  Source:")
    print(f"    General   : {SRC}")
    print(f"    Library   : {LIB_SRC}")
    print(f"    Tests     : {TESTS_SRC}")
    print("  Binaries:")
    print(f"    General   : {BIN}")
    print(f"    Library   : {LIB_BIN}")
    print(f"      Archives: {ARCHIVES}")
    print(f"    Tests     : {TESTS_BIN}")
    print("")

    # If given, clean first
    if args.clean:
        clean()
        print("")

    # Run the appropriate action
    actions.build(action, os, args.threaded)

    print("\nDone.\n")