package pacman;

public class PacmanStateFactory {
	
	public static PacmanState createPacman(PacmanState.State state, Pacman pacman) {
		switch(state) {
			case SUPERPACMAN:
				return new PacmanSuperState(pacman);
			case INVISIBLE:
				return new PacmanInvisibleState(pacman);
			default:
				return new PacmanNormalState(pacman);
			}
	}
}
