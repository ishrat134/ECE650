import sys
import re
from calculations_modified import *


def check_extra_space(ll):
    data = ll.split('"', 2)
    street_name = data[1]
    s = street_name.split(" ")
    if s.__contains__(''):
        print("Error: Extra spaces between street name words not allowed ")

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
            print("Execution Completed")
            sys.stdout.flush()

            break

        try:
            process_input(line, input, output, err)
        except ThrowException as ex:
            print("Error:")


def process_input(line, input, output, err):
    if not line.startswith('a') and not line.startswith('m') and not line.startswith('r') and not line.startswith('g'):
        print("Error: not a valid input ")


    else:

        regex_add_street = re.compile(r'^add\s\"[A-Za-z]+(?: +[A-Za-z]+)*\"(\s\(-?[0-9]+,-?[0-9]+\)){2,}$')
        regex_mod_street = re.compile(r'^mod\s\"[A-Za-z]+(?: +[A-Za-z]+)*\"(\s\(-?[0-9]+,-?[0-9]+\)){2,}$')
        regex_remove_street = re.compile(r'^rm\s\"[A-Za-z]+(?: +[A-Za-z]+)*\"$')
        regex_street_name = re.compile(r'^[A-Za-z]+(?: +[A-Za-z]+)*$')
        regex_gg = re.compile(r'^gg$')
        input_data = line.split(" ", 1)
        if len(input_data) < 2 and line != "gg":
            print("Error:  insufficient data ")

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
                    print("Error:  entered operation is incorrect ")


                else:
                    validate_street_name_coordinate(input_data)

            elif line.startswith('m') and check_extra_space(input_data[1]) == "no":
                if regex_mod_street.match(line):
                    street_name = fetch_street_name(input_data[1])
                    coordinates_street = fetch_street_coordinates(input_data[1])
                    modify_street(street_name, coordinates_street)
                elif operation not in allowed_opts:
                    print("Error: entered operation is incorrect ")

                else:
                    validate_street_name_coordinate(input_data)

            elif line.startswith('r'):
                if regex_remove_street.match(line) and check_extra_space(input_data[1]) == "no":
                    street_name = fetch_street_name(input_data[1])
                    remove_street(street_name)
                elif operation not in allowed_opts:
                    print("Error:  entered operation is incorrect ")

                else:
                    street_name = fetch_street_name(input_data[1])
                    if regex_street_name.match(street_name):
                        print("Error: No special character allowed as street name ")

            elif line.startswith('g'):
                if len(input_data) >= 2:
                    print("Error:  entered operation is incorrect ")

        elif regex_gg.match(line):
            vertex_dict, edges = generate_graph()
            v_size=len(vertex_dict)


            if (v_size>0):
                sys.stdout.write("V {0}\n".format(len(vertex_dict)))
                st_e = 'E {'
                for i in range(0, len(edges)):
                    if i < len(edges) - 1:
                        st_e += "<" + str(edges[i][0]) + "," + str(edges[i][1]) + ">,"
                    else:
                        st_e += "<" + str(edges[i][0]) + "," + str(edges[i][1]) + ">"

                st_e += "}\n"
                sys.stdout.write(st_e)
                sys.stdout.flush()
            else:
                sys.stderr.write("V 0\nE {}\nError: No edges found\n")



def validate_street_name_coordinate(input_data):
    # check for space b/w street name and co-ordinates
    splitup = input_data[1].split('"(', 1)
    if len(splitup) > 1:
        print("Error:  Missing Space between street and coordinates ")
        sys.stdout.flush()
        return

    # check for space b/w two co-ordinates
    splitup = input_data[1].split(')(', 1)
    if len(splitup) > 1:
        print("Error: Missing space between two coordinates ")

        return

    # check for street details
    street_data = input_data[1].split('"', 2)
    if (len(street_data) == 3):
        street_name = street_data[1].lower()
        if not re.match(r'^[A-Za-z]+(?: +[A-Za-z]+)*$', street_name):
            print("Error:  No special character allowed as street name ")

            return

        street_data[2] = street_data[2].strip().replace("\n", '')
        # check for missing paranthesis

        count_open = street_data[2].strip().count('(')
        count_closed = street_data[2].strip().count(')')

        if count_closed != count_open:
            print("Error:  Missing paranthesis ")

            return

        # improper co-ordinates
        tup = tuple(street_data[2].strip().split(" "))

        if (len(tup) < 2):
            print("Error:  insufficient data ")

        regex_coord = re.compile(r'^\(-?[0-9]+,-?[0-9]+\)$')
        for i in tup:
            if not regex_coord.match(i):
                print("Error: co-ordinates not entered properly ")

                return
    else:
        print("Error:  Insufficient data ")

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
