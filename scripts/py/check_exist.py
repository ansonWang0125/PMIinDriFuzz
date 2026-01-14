import sys
import argparse
import json
import os
from pathlib import Path

class CheckEnvExist(object):

    def __init__(self):
        self.__set_constant_member()

    def __set_constant_member(self):
        self.type_env = "env"
        self.type_file = "file"
        self.type_list = ["env", "file", "path"]

    def __check_env_exist(self, env):
        if env not in os.environ:
            print("Environment variable %s not exist." % (env))
            return False
        return True

    def __check_file_exist(self, file):
        if not os.path.isfile(file):
            print("File %s not exist." % (file))
            return False
        return True

    def __check_path_exist(self, path):
        if not os.path.exists(path):
            print("Dir %s not exist." % (path))
            return False
        return True
    
    def process(self, target, target_type):
        if target_type not in self.type_list:
            print("Please provide a valid type. option:")
            print(self.type_list)
        if target_type == self.type_env:
            if self.__check_env_exist(target):
                return os.environ[target]
        elif self.type_file:
            if self.__check_file_exist(target):
                return target
        elif self.type_file:
            if self.__check_path_exist(target):
                return target
        return None
