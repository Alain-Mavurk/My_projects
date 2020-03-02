package pacman;

import javax.swing.JOptionPane;

public class App {
	private static final int GAME_SPEED = 100;

	public static void main(String[] args) {
		
		Game game = new Game();
		Gui gui = new Gui(game);
		
		int continuer = 0;
		
		do {
			
			game.getPacman().addObserver(gui);
			
			for (Ghost ghost : game.getGhosts()) {
				ghost.addObserver(gui);
			}

			do {
				game.step();
				try {
					Thread.sleep(GAME_SPEED);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}while(!game.gameEnd());
			
			if(game.win()) {
				JOptionPane.showMessageDialog(gui.getComponent(), "Vous avez gagné");
			}else {
				JOptionPane.showMessageDialog(gui.getComponent(), "Vous avez perdu");
			}
			
			continuer = JOptionPane.showConfirmDialog(gui.getComponent(), "Voulez-vous continuer ?", "", JOptionPane.YES_NO_OPTION);
			
			game = new Game();
			gui.setGame(game);
			
		}while(continuer == JOptionPane.YES_OPTION);
		
		gui.dispose();
		
	}

}
