package uk.ac.ncl.undergraduate.modules.csc3621.CPA;
/**
 * You don't need to modify this class.
 * @author Changyu Dong
 *
 */
public class Judge {
	
	public static int test() {
		int totalWins = 0;
		
		for(int i=0;i<80;i++) {
			Challenger orc = new Challenger();
			Adversary cha = new Adversary();
			
			cha.run(orc);
			
			if(orc.result()) {
				totalWins++;
			}
			
		}
		
		if(totalWins == 80) {
			System.out.println("The adversary beat the challenger in the CPA game.");
		}else {
			System.out.println("The adversary failed. Total wins = "+totalWins+"/80.");
		}
		return totalWins;
		
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		//run 80 games, you pass if you can win them all.
		
		Judge jdg = new Judge();
		jdg.test();
		

	}

}
