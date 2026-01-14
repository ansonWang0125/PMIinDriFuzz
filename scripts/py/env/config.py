from pathlib import Path
import types
from environment import Environment
import json

class Config(object):

    def __init__(self):
        self.__set_constant()
        self.env = Environment()
    
    def __set_constant(self):
        self.__parse_config()

    def __parse_config(self):
        config_path = self.env.config_str / "fuzz_instance.json"
        with open(config_path) as fd:
            self.config = json.load(fd)


