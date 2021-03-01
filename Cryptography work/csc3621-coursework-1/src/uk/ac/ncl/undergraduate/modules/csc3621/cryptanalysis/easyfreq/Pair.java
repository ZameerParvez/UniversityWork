package uk.ac.ncl.undergraduate.modules.csc3621.cryptanalysis.easyfreq;

/**
 * This class is for generic tuples of size 2
 *
 * @author Zameer Parvez
 */
public class Pair<T1, T2> {
	private final T1 a;
	private final T2 b;
	
	public Pair(T1 u, T2 v) {
		this.a = u;
		this.b = v;
	}
	
	public T1 getKey() {
		return a;
	}
	
	public T2 getValue() {
		return b;
	}

	@Override
	public String toString() {
		return "{" +
				"a=" + a +
				", b=" + b +
				'}';
	}
}
