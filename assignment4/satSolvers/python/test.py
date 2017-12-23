from cnf2sat import satisfiable

def stringifyVar(var):
	strVar = "X"+str(abs(var))
	prepend = "-" if var<0 else ""
	return prepend+strVar

def test(expression, expected):
	res = satisfiable(expression)
	print "Clause:",
	for i in range(len(expression)):
		clause = expression[i]
		print "("+stringifyVar(clause[0])+" or "+stringifyVar(clause[1])+")",
		if i < len(expression)-1:
			print "and",
		else:
			print ""
	
	print "Expected: " + str(expected) + "\nActual: " + str(res) + "\n"

def main():
	expression = [(-1, -2), (1, 2)]
	test(expression,True)

	expression = [(-1,-1), (1,1)]
	test(expression, False)

main()
