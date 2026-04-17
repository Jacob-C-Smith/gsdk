def parse(data, port, origin):
    # if port==3001:
    #     return
    # if origin == 'client':
    #     return
    print(str("[{}({})] {}").format(origin, port, data[:100].hex()))