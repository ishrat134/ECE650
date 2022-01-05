import sys
import re
from calculations_modified import *


def check_extra_space(ll):
    data = ll.split('"', 2)
    street_name = data[1]
    s = street_name.split(" ")
    if s.__contains__(''):
        print("Error: Extra spaces between street name words not allowed ", file=sys.stderr)
        return "yes"
    return "no"


def fetch_street_name(ll):
    data = ll.split('"', 2)
    street_name = data[1]
    street_name = street_name.replace(" ", "_")
    street_name = street_name.lower()
    return street_name


def fetch_street_coordinates(ll):
    data = ll.split('"', 2)
    list_cordinates = []
    string_coordinates = data[2].strip().split(" ")
    for i in string_coordinates:
        coord = eval(i)
        if coord not in list_cordinates:
            list_cordinates.append(coord)
    return list_cordinates



def begin(input, output, err):
    while True:
        try:
            line = input.readline()
            line = line.strip().replace('\n', '')
        except EOFError:
            break

        if line == "":
            break

        elif line == "DONE" or line == "done":
            print("Execution Completed", file=output)
            output.flush()
            break

        try:
            process_input(line, input, output, err)
        except ThrowException as ex:
            print("Error: {0}\n".format(ex), file=err)


def process_input(line, input, output, err):
    if not line.startswith('a') and not line.startswith('m') and not line.startswith('r') and not line.startswith('g'):
        print("Error: not a valid input ", file=err)

    else:

        regex_add_street = re.compile(r'^add\s\"[A-Za-z]+(?: +[A-Za-z]+)*\"(\s\(-?[0-9]+,-?[0-9]+\)){2,}$')
        regex_mod_street = re.compile(r'^mod\s\"[A-Za-z]+(?: +[A-Za-z]+)*\"(\s\(-?[0-9]+,-?[0-9]+\)){2,}$')
        regex_remove_street = re.compile(r'^rm\s\"[A-Za-z]+(?: +[A-Za-z]+)*\"$')
        regex_street_name = re.compile(r'^[A-Za-z]+(?: +[A-Za-z]+)*$')
        regex_gg = re.compile(r'^gg$')
        input_data = line.split(" ", 1)
        if len(input_data) < 2 and line != "gg":
            print("Error:  insufficient data ", file=err)
            return
        operation = input_data[0]
        allowed_opts = ["add", "mod", "rm"]
        if not regex_gg.match(line):
            if line.startswith('a'):
                if regex_add_street.match(line) and check_extra_space(input_data[1]) == "no":
                    street_name = fetch_street_name(input_data[1])
                    coordinates_street = fetch_street_coordinates(input_data[1])
                    add_street(street_name, coordinates_street)

                elif operation not in allowed_opts:
                    print("Error:  entered operation is incorrect ", file=err)

                else:
                    validate_street_name_coordinate(input_data)

            elif line.startswith('m') and check_extra_space(input_data[1]) == "no":
                if regex_mod_street.match(line):
                    street_name = fetch_street_name(input_data[1])
                    coordinates_street = fetch_street_coordinates(input_data[1])
                    modify_street(street_name, coordinates_street)
                elif operation not in allowed_opts:
                    print("Error: entered operation is incorrect ", file=err)
                else:
                    validate_street_name_coordinate(input_data)

            elif line.startswith('r'):
                if regex_remove_street.match(line) and check_extra_space(input_data[1]) == "no":
                    street_name = fetch_street_name(input_data[1])
                    remove_street(street_name)
                elif operation not in allowed_opts:
                    print("Error:  entered operation is incorrect ", file=err)
                else:
                    street_name = fetch_street_name(input_data[1])
                    if regex_street_name.match(street_name):
                        print("Error: No special character allowed as street name ", file=err)

            elif line.startswith('g'):
                if len(input_data) >= 2:
                    print("Error:  entered operation is incorrect ", file=err)

        elif regex_gg.match(line):
            vertex_dict, edges = generate_graph()
            print(format('V = {' + '\n'), file=output)
            for key, value in vertex_dict.items():
                print(str(key) + ": " + str(value) + "\n", file=output)
            print('}', file=output)
            print('\n', file=output)
            output.flush()
            print(format('E = {' + '\n'), file=output)
            for i in range(0, len(edges)):
                if i < len(edges) - 1:
                    print("<" + str(edges[i][0]) + "," + str(edges[i][1]) + ">," + '\n', file=output)
                else:
                    print("<" + str(edges[i][0]) + "," + str(edges[i][1]) + ">", file=output)
            print("\n" + "}", file=output)
            output.flush()


def validate_street_name_coordinate(input_data):
    # check for space b/w street name and co-ordinates
    splitup = input_data[1].split('"(', 1)
    if len(splitup) > 1:
        print("Error:  Missing Space between street and coordinates ", file=sys.stderr)
        return

    # check for space b/w two co-ordinates
    splitup = input_data[1].split(')(', 1)
    if len(splitup) > 1:
        print("Error: Missing space between two coordinates ", file=sys.stderr)
        return

    # check for street details
    street_data = input_data[1].split('"', 2)
    if (len(street_data) == 3):
        street_name = street_data[1].lower()
        if not re.match(r'^[A-Za-z]+(?: +[A-Za-z]+)*$', street_name):
            print("Error:  No special character allowed as street name ", file=sys.stderr)
            return

        street_data[2] = street_data[2].strip().replace("\n", '')
        # check for missing paranthesis

        count_open = street_data[2].strip().count('(')
        count_closed = street_data[2].strip().count(')')

        if count_closed != count_open:
            print("Error:  Missing paranthesis ", file=sys.stderr)
            return

        # improper co-ordinates
        tup = tuple(street_data[2].strip().split(" "))

        if (len(tup) < 2):
            print("Error:  insufficient data ", file=sys.stderr)
        regex_coord = re.compile(r'^\(-?[0-9]+,-?[0-9]+\)$')
        for i in tup:
            if not regex_coord.match(i):
                print("Error: co-ordinates not entered properly ", file=sys.stderr)
                return
    else:
        print("Error:  Insufficient data ", file=sys.stderr)
        return


class ThrowException(Exception):
    def __init__(self, value):
        self._value = value

    def __str__(self):
        return str(self._value)


def main():
    begin(sys.stdin, sys.stdout, sys.stderr)


if __name__ == '__main__':
    main()
