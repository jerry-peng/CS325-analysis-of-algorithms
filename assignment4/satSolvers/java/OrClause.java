
public class OrClause {
    public int first;
    public int second;

	public OrClause(int f, int s) {
		first = f;
		second = s;
	}

	public OrClause(OrClause clause) {
		first = clause.first;
		second = clause.second;
	}
}
