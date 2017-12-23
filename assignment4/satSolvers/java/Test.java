
public class Test {
	public static void main(String[] args) {
		CNF2SAT cnf2sat = new CNF2SAT(new OrClause[] { new OrClause(1, -2), new OrClause(2, -1) });
		System.out.println("Expected: true, Actual: "+cnf2sat.satisfiable());

		cnf2sat = new CNF2SAT(new OrClause[] { new OrClause(2, 2), new OrClause(-2, -2) });
		System.out.println("\nExpected: false, Actual: "+cnf2sat.satisfiable());
	}
}
