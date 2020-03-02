package pacman;

import java.awt.event.ActionEvent;
import java.util.ArrayList;

import javax.swing.AbstractAction;
import javax.swing.JDialog;
import javax.swing.JOptionPane;
import javax.swing.Timer;

public class Game {
	
	private static final Coordinate PACMAN_START_POSITION = new Coordinate(1, 29);
	private static final Coordinate GHOST_START_POSITION = new Coordinate(14, 14);
	private ArrayList<Character> characters;
	private Pacman pacman;
	private ArrayList<Ghost> ghosts;
	private Maze maze;
	private ArrayList<Pacdot> pacdots;
	private int width;
	private int height;
	private int score;
	
	public Game() {
		maze = new Maze();
		pacman = new Pacman(this, PACMAN_START_POSITION);
		ghosts = new ArrayList<Ghost>();
		pacdots = new ArrayList<Pacdot>();
		characters = new ArrayList<Character>();
		startGhostPosition();
		generatePacdots();
		addCharacters();
		width = Maze.getWidth();
		height = Maze.getHeight();
		score = 0;
	}
	
	private void addCharacters() {
		characters.add(pacman);
		for (Ghost ghost : ghosts) {
			characters.add(ghost);
		}
	}

	public Maze getMaze() {
		return maze;
	}

	public ArrayList<Pacdot> getPacdots() {
		return pacdots;
	}

	public ArrayList<Ghost> getGhosts() {
		return ghosts;
	}
	
	public int getHeight() {
		return height;
	}

	public int getWidth() {
		return width;
	}
	
	public Pacman getPacman() {
		return pacman;
	}
	
	public int getScore() {
		return score;
	}
	
	public boolean isWall(int i, int j) {
		return maze.isWall(i, j);
	}
	
	public boolean isDoor(int i, int j) {
		return maze.isDoor(i, j);
	}
	
	public boolean isPacdot(int i, int j) {
		return maze.isPacdot(i, j);
	}
	
	public boolean win() {
		return pacdots.isEmpty();
	}
	
	public boolean die() {
		return !pacman.isAlive();
	}
	
	public boolean gameEnd() {
		return win() || die();
	}
	
	public void incScore(Pacdot pacdot) {
		score += pacdot.getPoints();
		if(score == 5000) {
			pacman.incLife();
		}
	}
	
	public void removePacdot(Pacdot pacdot) {
		pacdots.remove(pacdot);
	}
	
	public void startGhostPosition() {
		for (int i = 0; i < 4; i++) {
			ghosts.add(new Ghost(this, GHOST_START_POSITION));
		}
	}
	
	public void generatePacdots() {
		int length = maze.getBoard().length;
		int [][] board = maze.getBoard();
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < board[0].length; j++) {
				if(isPacdot(i, j))
					pacdots.add(PacdotFactory.createPacdot(this, new Coordinate(j, i), board[i][j]));
			}
		}
	}
	
	public boolean wallAndDoorCollision(Coordinate c) {
		int i = c.getY();
		int j = c.getX();
		return isWall(i, j) || isDoor(i, j);
	}
	
	public boolean ghostCollisionOnStop(Ghost ghost, Coordinate c) {
		return ghost.getPosition().equals(c);
	}
	
	public boolean ghostCollisionOnMove(Ghost ghost) {
		int directionX = ghost.getDirection().getX() + pacman.getDirection().getX();
		int directionY = ghost.getDirection().getY() + pacman.getDirection().getY();
		boolean equals = (directionX == 0) && (directionY == 0);
		return ghost.getPosition().equals(pacman.nextPosition()) && equals;
	}
	
	public boolean ghostCollision(Coordinate c) {
		boolean collision = false;
		for (Ghost ghost : ghosts) {
			if(ghostCollisionOnMove(ghost) || ghostCollisionOnStop(ghost, c)) {
				pacman.ghostCollision(ghost);
				collision = true;
			}
		}
		return collision;
	}
	
	public void checkPacmanCollision(Ghost ghost) {
		if(ghost.getPosition().equals(pacman.getPosition()))
			pacman.ghostCollision(ghost);
	}
	
	
	public void checkPacdotCollision(Coordinate c) {
		for (Pacdot pacdot : pacdots) {
			if(pacdot.getPosition().equals(c)) {
				pacman.eat(pacdot);
				break;
			}
		}
	}
	
	public void setPacmanDirection(Direction direction) {
		if(wallAndDoorCollision(pacman.nextPosition(direction)))
			pacman.setMemoryDirection(direction);
		else
			pacman.setDirection(direction);
	}
	
	boolean isOut(Coordinate c) {
		if (c.getX() < 0 || c.getY() < 0)
			return true;
		if (c.getX() >= width || c.getY() >= height)
			return true;
		return false;
	}
	
	public void wraparound(Coordinate c) {
		if(c.getX() < 0) {
			pacman.setPosition(new Coordinate(width, c.getY()));
		}else if(c.getY() < 0) {
			pacman.setPosition(new Coordinate(c.getX(), height));
		}else if(c.getX() >= width) {
			pacman.setPosition(new Coordinate(-1, c.getY()));
		}else {
			pacman.setPosition(new Coordinate(c.getX(), -1));
		}
	}
	
	public void showPacmanLife() {
		JOptionPane showMessage = new JOptionPane("Nombre de vies restant: " + pacman.getLife(), JOptionPane.INFORMATION_MESSAGE, JOptionPane.DEFAULT_OPTION, null, new Object[]{}, null);
		final JDialog dialog = new JDialog();
		dialog.setTitle("Attention !!");
		dialog.setContentPane(showMessage);
		dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		dialog.pack();
		Timer timer = new Timer(1000, new AbstractAction() {
			private static final long serialVersionUID = 1L;

			@Override
		    public void actionPerformed(ActionEvent ae) {
		        dialog.dispose();
		    }
		});
		timer.setRepeats(false);
		timer.start();
		dialog.setVisible(true);
	}
	
	public void reset() {
		pacman.lostLife();
		showPacmanLife();
		pacman.initPosition();
		pacman.checkLife();
		for (Ghost ghost : ghosts) {
			ghost.changeState(GhostStateFactory.createGhost(GhostState.State.PRISON, ghost));
		}
	}
	
	public void step() {
		for (Character character : characters) {
			character.move();
		}
	}

	
}
