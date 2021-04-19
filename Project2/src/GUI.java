import javax.swing.*;
import java.awt.*;

/*
    Class to display and update panels of the GUI
 */
public class GUI {
    JPanel cPane;
    JPanel lPane;
    JPanel namePane;

    JTextArea upNext;
    JTextArea timeField;

    JTextField first[];
    JTextField worst[];
    JTextField best[];

    public void displayGUI(){
        JFrame frame = new JFrame("Project Two");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800,800);

        cPane = new JPanel();
        lPane = new JPanel();
        namePane = new JPanel();

        upNext = new JTextArea("Next Process Size to be Allocated: ");
        upNext.setColumns(10);
        upNext.setEditable(false);
        timeField = new JTextArea("Time Units Passed: ");
        timeField.setColumns(10);
        timeField.setEditable(false);


        cPane.setLayout(new GridLayout(16, 3));
        lPane.setLayout(new GridLayout(2, 1));

        first = new JTextField[15];
        best = new JTextField[15];
        worst = new JTextField[15];

        JTextField f = new JTextField("First Fit");
        JTextField b = new JTextField("Best Fit");
        JTextField w = new JTextField("Worst Fit");
        JTextField name = new JTextField("CIS 452 Project Two");
        name.setEditable(false);

        for (int i = 0; i < 15; i++) {
            first[i] = new JTextField("");
            first[i].setColumns(5);
            first[i].setBackground(Color.ORANGE);

            best[i] = new JTextField("");
            best[i].setColumns(5);
            best[i].setBackground(Color.ORANGE);

            worst[i] = new JTextField("");
            worst[i].setColumns(5);
            worst[i].setBackground(Color.ORANGE);

            cPane.add(first[i]);
            cPane.add(best[i]);
            cPane.add(worst[i]);
        }

        cPane.add(f);
        cPane.add(b);
        cPane.add(w);
        lPane.add(timeField);
        lPane.add(upNext);
        namePane.add(name);

        frame.add(lPane, BorderLayout.WEST);
        frame.add(cPane);
        frame.add(namePane, BorderLayout.NORTH);
        frame.setVisible(true);
    }

    public void updateColumn(int size, int id){

    }

    public void updateTime(){

    }

    public void updateNext(int size){
        upNext.setText("Next Process Size to be Allocated: \n"
                + "First: " + size + "\n"
                + "Best: " + size + "\n"
                + "Worst: " + size + '\n');
        upNext.revalidate();
        upNext.repaint();
    }
}
