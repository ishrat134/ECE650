import sys


import math

street_dictionary = {}
intersections = []


def cust_function(street_a, street_b, inter_coord, point1, point2, point3, point4, list_vertex_reference):
    list_vertex_reference = support(street_a, inter_coord, point1, point2, list_vertex_reference)
    list_vertex_reference = support(street_b, inter_coord, point3, point4, list_vertex_reference)

    return list_vertex_reference


def tuple_to_float(point):
    point = tuple([float("{0:.2f}".format(n)) for n in point])
    return point;


def get_intersection(p1, p2, p3, p4):
    a1, b1, c1 = get_line(p1, p2)
    a2, b2, c2 = get_line(p3, p4)

    if a1==0:
        return -1000, -1000

    if a2 ==0:
        return -1000, -1000

    if -(b1 / a1) == -(b2 / a2):
        return -1000, -1000

    if (a1 *b2 - a2 * b1 != 0):
        x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1)
        y = (c1 * a2 - c2 * a1) / (a1 * b2 - a2 * b1)
        return x, y

    return  -1000,-1000


def calculate_vertex():
    list_vertex_street_reference = {}
    global street_dictionary
    list_vertex = {}
    global intersections
    index = 1
    for k1, v1 in street_dictionary.items():
        street_a = k1
        i = 0
        while i < (len(street_dictionary[street_a]) - 1):
            point1 = street_dictionary[street_a][i]
            point2 = street_dictionary[street_a][i + 1]

            for k2, v2 in street_dictionary.items():
                j = 0
                if k2 == street_a:
                    continue
                else:
                    street_b = k2
                    while j < (len(street_dictionary[street_b]) - 1):
                        point3 = street_dictionary[street_b][j]
                        point4 = street_dictionary[street_b][j + 1]
                        x_inter, y_inter = get_intersection(point1, point2, point3, point4)
                        if x_inter != -1000 and y_inter != -1000:
                            inter_coord = (x_inter, y_inter)
                            if is_between(point1, inter_coord, point2) and is_between(point3, inter_coord, point4):
                                point1 = tuple_to_float(point1)
                                point2 = tuple_to_float(point2)
                                point3 = tuple_to_float(point3)
                                point4 = tuple_to_float(point4)
                                inter_coord = tuple_to_float(inter_coord)

                                temp = [point1, inter_coord, point2, point3, inter_coord, point4]
                                intersections.append(inter_coord)
                                for k in range(0, len(temp)):
                                    if temp[k] not in list_vertex.values():
                                        list_vertex[index] = temp[k]
                                        list_vertex_street_reference = cust_function(street_a, street_b, inter_coord,
                                                                                     point1,
                                                                                     point2, point3, point4,
                                                                                     list_vertex_street_reference)
                                        index = index + 1
                        j = j + 1
            i = i + 1

    return list_vertex, list_vertex_street_reference


def get_line(p1, p2):
    a = p2[1] - p1[1]
    b = p1[0] - p2[0]
    c = -(a * (p1[0]) + b * (p1[1]))
    return a, b, c


def is_between(a, c, b):
    return round(distance(a, c) + distance(c, b), 2) == round(distance(a, b), 2)


def distance(a, b):
    return math.sqrt((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2)


def support(street, inter_coord, point1, point2, list_vertex_street_reference):
    if street in list_vertex_street_reference:
        ll = list_vertex_street_reference[street]
        if inter_coord not in ll[-1]:
            ll.append(point1)
            ll.append(inter_coord)
            ll.append(point2)

            if (type(ll) != "list"):
                ll = ll[:-3] + [ll[-3:]]

        list_vertex_street_reference[street] = ll
    else:
        temp_list = [point1, inter_coord, point2]
        list_vertex_street_reference[street] = [temp_list]
    return list_vertex_street_reference


def calculate_edges(vertex_dictionary, list_vertex_street_reference):
    global street_dictionary
    list_edges = []
    list_edges_points = []

    for key, value in list_vertex_street_reference.items():

        ans = check(key, value)
        if ans:
            ll, edges_list = get_pairs_without_traverse(value, vertex_dictionary)
            for i in ll:
                list_edges.append(i)
            list_edges_points.append(edges_list)

        else:
            for v in value:
                point1 = v[0]
                point2 = v[2]
                intersec = v[1]
                if is_between(point1, intersec, point2):
                    get_index_point1 = get_key(vertex_dictionary, point1)
                    get_index_intersec = get_key(vertex_dictionary, intersec)
                    get_index_point2 = get_key(vertex_dictionary, point2)

                    list_edges_points.append((point1, intersec))
                    list_edges_points.append((intersec, point2))
                    list_edges.append([get_index_point1, get_index_intersec])
                    list_edges.append([get_index_intersec, get_index_point2])
    return list_edges


def check(key, value):
    for i in range(0, len(value)):
        strt1 = value[i][0]
        end1 = value[i][2]
        for j in range(i + 1, len(value)):
            strt2 = value[j][0]
            end2 = value[j][2]
            if strt1 == strt2 and end1 == end2:
                return True
    return False


def get_pairs_without_traverse(temp_list, vertex_dictionary):
    flat_list = []
    edge_list = []
    finak_edge_list = []
    for sublist in temp_list:
        for item in sublist:
            if item not in flat_list:
                flat_list.append(item)
    sorted_list = sorted(flat_list, key=lambda x: [x[1], x[0]])
    for j in range(0, len(sorted_list) - 1):
        pair = [sorted_list[j], sorted_list[j + 1]]
        edge_list.append(pair)

    for pair in edge_list:
        point1 = pair[0]
        point2 = pair[1]
        ind_point1 = get_key(vertex_dictionary, point1)
        ind_point2 = get_key(vertex_dictionary, point2)
        finak_edge_list.append([ind_point1, ind_point2])
    return finak_edge_list, edge_list


def get_key(vertex_dictionary, val):
    for key, value in vertex_dictionary.items():
        if val == value:
            return key

    return "key doesn't exist"


def generate_graph():
    global street_dictionary
    vertex_dictionary, list_vertex_street_reference = calculate_vertex()
    edges = calculate_edges(vertex_dictionary, list_vertex_street_reference)
    return vertex_dictionary, edges


def modify_street(street, coordinates):
    global street_dictionary
    if street in street_dictionary:
        street_dictionary[street] = coordinates
    else:
        print("Error: No such street exists, please enter an existing street name")
        sys.stdout.flush()


def add_street(street, coordinates):
    global street_dictionary
    if street in street_dictionary:
        print("Error: street already exists, you cannot add it again, try to modify")
        sys.stdout.flush()
    street_dictionary[street] = coordinates


def remove_street(street):
    global street_dictionary
    if street in street_dictionary:
        del street_dictionary[street]
    else:
        print("Error: No such street exists, please enter an existing street name")
        sys.stdout.flush()
