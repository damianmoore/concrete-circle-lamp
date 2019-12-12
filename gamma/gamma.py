# Creates a gamma-corrected lookup table
# https://gist.github.com/hexagon5un/3df734ad08d8dc8d9ace0491ef97cc58

import math


def gamma(nsteps, gamma):
    gammaedUp = [math.pow(x, gamma) for x in range(nsteps)]
    return [x/max(gammaedUp) for x in gammaedUp]

def rounder(topValue, gammas):
    return [min(topValue, round(x*topValue)) for x in gammas]

if __name__ == "__main__":
    myGamma = 2.8
    steps = 255
    output = file("gamma.h", "w")
    output.write("/* %d-step brightness table: gamma = %s */ \n\n" % (steps, myGamma))
    output.write("const uint8_t gamma_table[%d] = {\n\t" % steps)
    for value in rounder(255, gamma(steps, myGamma)):
        output.write("%d, " % value)
    output.write("\n};\n")
output.close()