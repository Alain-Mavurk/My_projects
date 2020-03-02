package pacman;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Toolkit;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JComponent;
import javax.swing.JFrame;

import pacman.Pacdot.Type;




public class Gui implements Observer{

	private static final int SCALE = 20;

	protected static final int INFO_PLAYER = 4;

	private static final Color PURPLE = new Color(186, 85, 211);
	
	private Game game;
	private JFrame frame = new JFrame("Pac-man");
	private static Map<PacmanState.State, Color> pacmanColor = new HashMap<PacmanState.State, Color>();
	private static Map<GhostState.State, Color> ghostColor = new HashMap<GhostState.State, Color>();
	private static Map<Pacdot.Type, Color> pacdotColor = new HashMap<Pacdot.Type, Color>();
	private static Map<Integer, Color> mazeColor = new HashMap<Integer, Color>();
	
	static {
		pacmanColor.put(PacmanState.State.NORMAL, Color.YELLOW);
		pacmanColor.put(PacmanState.State.INVISIBLE, new Color(230, 255, 100));
		pacmanColor.put(PacmanState.State.SUPERPACMAN, Color.ORANGE);
		
		ghostColor.put(GhostState.State.NORMAL, Color.CYAN);
		ghostColor.put(GhostState.State.PRISON, Color.WHITE);
		ghostColor.put(GhostState.State.VULNERABLE, Color.BLUE);
		
		pacdotColor.put(Type.NORMAL, Color.BLUE);
		pacdotColor.put(Type.SUPER, Color.ORANGE);
		pacdotColor.put(Type.MAZE, Color.GREEN);
		pacdotColor.put(Type.INVISIBLE, PURPLE);
		
		mazeColor.put(Maze.NORMAL_PACDOT, Color.BLACK);
		mazeColor.put(Maze.WALL, Color.DARK_GRAY);
		mazeColor.put(Maze.DOOR, Color.RED);
		mazeColor.put(Maze.PATH, Color.BLACK);
		mazeColor.put(Maze.MAZE_PACDOT, Color.BLACK);
		mazeColor.put(Maze.SUPER_PACDOT, Color.BLACK);
		mazeColor.put(Maze.INVISIBLE_PACDOT, Color.BLACK);
	}
	
	private JComponent component = new JComponent() {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;

		@Override
		protected void paintComponent(Graphics g) {
			super.paintComponent(g);
			g.setColor(Color.BLACK);
			g.fillRect(0, 0, game.getWidth()*SCALE, (game.getHeight()+INFO_PLAYER)*SCALE);
			int [][] board = game.getMaze().getBoard();
			draw(g, board);
			Toolkit.getDefaultToolkit().sync();
		}
		
		private void draw(Graphics g, int [][] board) {
			drawScore(g);
		    drawBoard(g, board);
			drawLife(g);
		}
		
		private void drawLife(Graphics g) {
			g.setColor(Color.YELLOW);
			for(int i = 0; i < game.getPacman().getLife(); i++) {
				g.fillOval(i*SCALE, game.getHeight()*SCALE, SCALE, SCALE);
			}
		}
		
		private void drawScore(Graphics g) {
			g.setColor(Color.WHITE);
			String score =  "Score : " + game.getScore() + " ";
			int width = g.getFontMetrics().stringWidth(score);
			g.drawString(score , (game.getWidth())*SCALE-width, (game.getHeight()+1)*SCALE);
		}
		
		private void drawBoard(Graphics g, int [][]board) {
			drawWall(g, board);
			drawPacdots(g);
			drawPacman(g);
			drawGhost(g);
		}
		
		private void drawWall(Graphics g, int [][]board) {
			for (int i = 0; i < board.length; i++) {
				for (int j = 0; j < board[i].length; j++) {
					g.setColor(mazeColor.get(board[i][j]));
					g.fillRect(j*SCALE, i*SCALE, SCALE, SCALE);
				}
			}
		}
		
		private void drawPacman(Graphics g) {
			g.setColor(pacmanColor.get(game.getPacman().getPacmanState()));
			g.fillOval(game.getPacman().getPosition().getX()*SCALE, game.getPacman().getPosition().getY()*SCALE, SCALE, SCALE);
		}
		
		private void drawGhost(Graphics g) {
			for (Ghost ghost : game.getGhosts()) {
				g.setColor(ghostColor.get(ghost.getState()));
				g.fillOval(ghost.getPosition().getX()*SCALE, ghost.getPosition().getY()*SCALE, SCALE, SCALE);
			}
		}
		
		private void drawPacdots(Graphics g) {
			for (Pacdot pacdot : game.getPacdots()) {
				g.setColor(pacdotColor.get(pacdot.getType()));
				g.fillOval(pacdot.getPosition().getX()*SCALE+SCALE/4, pacdot.getPosition().getY()*SCALE+SCALE/4, SCALE/2, SCALE/2);
			}	
		}
	};
	
	public Gui(Game g) {
		this.game = g;
		frame.setContentPane(component);
		frame.setSize(game.getWidth() * SCALE, (game.getHeight()+INFO_PLAYER) * SCALE);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
		frame.setResizable(false);
		frame.addKeyListener(new KeyAdapter() {
			
			@Override
			public void keyPressed(KeyEvent e) {
				switch (e.getKeyCode()) {
				
					case KeyEvent.VK_DOWN:
						game.getPacman().setMemoryDirection(Direction.Down);
						game.setPacmanDirection(Direction.Down);
						break;
					case KeyEvent.VK_UP:
						game.getPacman().setMemoryDirection(Direction.Up);
						game.setPacmanDirection(Direction.Up);
						break;
					case KeyEvent.VK_RIGHT:
						game.getPacman().setMemoryDirection(Direction.Right);
						game.setPacmanDirection(Direction.Right);
						break;
					case KeyEvent.VK_LEFT:
						game.getPacman().setMemoryDirection(Direction.Left);
						game.setPacmanDirection(Direction.Left);
						break;
					default:
						break;
				}
			}
		});
	}
	
	public void setGame(Game game) {
		this.game = game;
	}

	public JComponent getComponent() {
		return component;
	}



	public void dispose() {
		frame.dispose();
	}

	@Override
	public void update() {
		component.repaint();		
	}

}
