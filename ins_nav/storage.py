import pickle

try:
    import yaml

    def to_yaml(data, filename):
        with open(filename, 'wb') as fd:
            # d = yaml.dumps(data)
            # fd.write(d)
            yaml.dump(data, fd)

        print(f">> Saved data to {filename}")

    def from_yaml(file):
        try:
            with open(file,"r") as fd:
                d = yaml.safe_load(fd)
        except FileNotFoundError as e:
            print(e)
            return None
        return d

except ImportError:
    def to_yaml(data, file):
        print("Please install pyyaml to use this")
        return None

    def from_yaml(file):
        print("Please install pyyaml to use this")
        return None



def to_pickle(data, filename):
    with open(filename, 'wb') as fd:
        d = pickle.dumps(data)
        fd.write(d)

    print(f">> Saved data to {filename}")

def from_pickle(filename):
    try:
        with open(filename, 'rb') as fd:
            data = pickle.load(fd)
    except FileNotFoundError as e:
        print(e)
        return None

    print(f">> Loaded data from {filename}")
    return data
