package pacman;


import pacman.PacmanState.State;

public class PacdotInvisible extends Pacdot {

	public PacdotInvisible(Game game, Coordinate position) {
		super(game, position);
	}

	@Override
	public void eat() {
		game.getPacman().changeState(PacmanStateFactory.createPacman(State.INVISIBLE, game.getPacman()));
		for (Ghost ghost : game.getGhosts()) {
			if(ghost.getState() == GhostState.State.VULNERABLE)
				ghost.changeState(GhostStateFactory.createGhost(GhostState.State.NORMAL, ghost));
		}
		game.incScore(this);
		game.removePacdot(this);
	}

	@Override
	public int getPoints() {
		return 300;
	}

	@Override
	public Type getType() {
		return Type.INVISIBLE;
	}
}
