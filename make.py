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
import hashlib
import json


### DEFINE GLOBALS ###

# Define the general source and bin folders
BIN = "bin/"
SRC = "src/"

# Define library folders
LIB_SRC = SRC + "lib/"
LIB_BIN = BIN + "bin/"
ARCHIVES_BIN = LIB_BIN + "archives/"

# Define tests
TESTS_SRC = "tests/src/"
TESTS_BIN = "tests/bin/"

# Define what folders need to be cleaned (this is all .out, .o and .a files)
TO_CLEAN = (BIN, TESTS_BIN)
TO_CLEAN_EXT = ("out", "exe", "a", "o")

# Define compilers
UNIX_CC = "g++"
WIN_CC = ""

# Define compiler flags
UNIX_CC_ARGS = "-std=c++17 -O2 -Wall -Wextra"
WIN_CC_ARGS = ""

# Define the rule for creating object files
UNIX_CC_


### TOOLS ###
def get_file_hash(path):
    hash_md5 = hashlib.md5()
    try:
        with open(path, "r") as f:
            for chunk in iter(lambda: f.read(4096), b""):
                hash_md5.update(chunk)
        return hash_md5.hexdigest()
    except FileNotFoundError:
        print(f"\nERROR: Unknown file '{path}'\n")
        exit(-1)


class Actions:
    """
        This class is the collection of possible actions. Simply add one here
        to automatically add it to the script. Note that a function determining
        an action has to be prefixed with 'ACTION_'
    """



    def __init__(self, hashes):
        # Collect the actions
        self._actions = {}
        for name in dir(self):
            if callable(getattr(self, name)) and name[:7] == "ACTION_":
                self._actions[name[7:].lower()] = name
        
        # Store the hashes
        self._hashes = hashes

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
            print(f"ERROR: Unknown build target '{action}'\n")
            return
        
        # Run the function
        return getattr(self, self._actions[action])(os, threaded)

    def ACTION_Raytracer(self, os, raytracer):
        """
            Builds the main raytracer executable. Everything in the Libraries
            list is added as a dependency and first compiled to a .o file in
            the LIB_BIN directory. To use archive files, specify a new key with
            target library files in the Archives dict that will be grouped and
            outputted in the ARCHIVES_BIN directory. Note that the separate .o
            files will not be added as dependency anymore.
        """

        Libraries = ["test.cpp"]

        print("Building raytracer...")

        print("  Building libraries...")

        for lib in Libraries:
            # Check if this needs updating
            path = LIB_SRC + lib
            h = get_file_hash(path)
            if not in self._hashes or self._hashes[path] != h:
                # Build the file into a .o file
                self._build_lib(path)
                # If we succeeded that, store the hash in the internal hash
                #   table
                self._hashes[path] = h

        print("Done")

    def _build_lib(self, path_to_build):
        # Run the CC compiler without linking




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

    # Declare the hash file default
    hash_path = "make_hash.json"

    # Parse the arguments first
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--threaded", help="Compiles the raytracer using threads (both UNIX and Windows systems)", action="store_true")
    parser.add_argument("-c", "--clean", help="If specified, cleans the output directory before compiling", action="store_true")
    parser.add_argument("-u", "--unix", help="Forces UNIX build. Note that this does required (an alias of) g++ to be installed.", action="store_true")
    parser.add_argument("-w", "--win", help="Forces Windows build. Note that this required (an alias of) <> to be installed.", action="store_true")
    parser.add_argument("-H", "--hashes", help="Path to the file containing the hashes that need to be build. If the file does not exist, then it will be created.")
    parser.add_argument("action", help="Specifies the action of the build. If omitted, builds the raytracer application.", nargs='?')

    args = parser.parse_args()
    if args.action:
        action = args.action
    if args.hashes:
        hash_path = args.hashes

    # Detect the OS
    for_os = platform.system()
    if for_os != "Windows" and for_os != "Linux":
        print(f"ERROR: Unsupported OS '{for_os}'\n")
        exit()

    # Override if given
    if args.win and args.unix:
        print("ERROR: Cannot specify both --win and --unix\n")
        exit(-1)
    elif args.unix:
        for_os = "Linux"
    elif args.win:
        for_os = "Windows"

    print("\n*** BUILD SCRIPT FOR RAYTRACER ***\n")

    # Load the json file
    print(f"Loading hashes from \"{hash_path}\"...", end="")
    hashes = {}
    if os.path.isfile(hash_path):
        # Parse as json
        with open(hash_path, "r") as f:
            hashes = json.load(f)

            # Do some checks
            for elem in hashes:
                if type(elem) != str:
                    print(f"\nERROR: Json file contains invalid top-level key '{elem}' (should be string)")
                    exit(-1)
                if type(hashes[elem]) != str:
                    print(f"\nERROR: Invalid hash found for key '{elem}': '{hashes[elem]}' (should be string)")
            print(f"Done (loaded {len(hashes)} hashes)\n")
    else:
        print("\n  File not found, got no hashes\n")
    
    # Initialize the actions
    actions = Actions(hashes)

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

    print(f"  Building for {for_os}")

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
    print(f"    General  : {SRC}")
    print(f"    Library  : {LIB_SRC}")
    print(f"    Tests    : {TESTS_SRC}")
    print("  Binaries:")
    print(f"    General  : {BIN}")
    print(f"    Library  : {LIB_BIN}")
    print(f"    Archives : {ARCHIVES_BIN}")
    print(f"    Tests    : {TESTS_BIN}")
    print("")

    # If given, clean first
    if args.clean:
        clean()
        print("")

    # Run the appropriate action
    actions.build(action, for_os, args.threaded)

    print("\nDone.\n")