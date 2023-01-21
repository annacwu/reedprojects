def sum_cubes(n):
    count = 1
    sum = 0
    while count < n + 1: 
        cube = count ** 3
        sum = sum + cube
        count = count + 1
    return sum
