package pacman;


public class PacdotSuper extends Pacdot {

	public PacdotSuper(Game game, Coordinate position) {
		super(game, position);
	}

	@Override
	public void eat() {
		game.getPacman().changeState(PacmanStateFactory.createPacman(PacmanState.State.SUPERPACMAN, game.getPacman()));
		for (Ghost ghost : game.getGhosts()) {
			if(ghost.getState() != GhostState.State.PRISON)
				ghost.changeState(GhostStateFactory.createGhost(GhostState.State.VULNERABLE, ghost));
		}
		game.incScore(this);
		game.removePacdot(this);
	}

	@Override
	public int getPoints() {
		return 500;
	}

	@Override
	public Type getType() {
		return Type.SUPER;
	}

}
