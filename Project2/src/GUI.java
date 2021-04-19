import javax.swing.*;
import java.awt.*;
import java.lang.reflect.Method;

/***
    Class to display and update panels of the GUI
 */
public class GUI {
    JPanel cPane;
    JPanel lPane;
    JPanel namePane;

    JTextArea upNext;
    JTextArea timeField;

    static JTextField first[];
    static JTextField worst[];
    static JTextField best[];

    public Method GetUpdateMethod() throws NoSuchMethodException {
        Method m = GUI.class.getDeclaredMethod("UpdateMemorySpace", int.class, String.class);
        return m;
    }


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
            first[i].setHorizontalAlignment(JTextField.CENTER);
            first[i].setBackground(Color.ORANGE);

            best[i] = new JTextField("");
            best[i].setColumns(5);
            best[i].setHorizontalAlignment(JTextField.CENTER);
            best[i].setBackground(Color.ORANGE);

            worst[i] = new JTextField("");
            worst[i].setColumns(5);
            worst[i].setHorizontalAlignment(JTextField.CENTER);
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

    /***
     * 0 First
     * 1 Best
     * 2 Worst
     * @param column
     */
    public static void UpdateMemorySpace(int column, String memSpace)
    {
        //System.out.println(memSpace);
        String s = memSpace;
        String s2 = "";
        s = s.substring(1, s.length()-1);

        if (column == 0){
            for (int j = 0; j < 15; j++){
                if(s.charAt(j) != '_') {
                    first[j].setText(s.charAt(j) + "");
                    first[j].setBackground(Color.pink);
                }
                else{
                    first[j].setBackground(Color.ORANGE);
                    first[j].setText("");
                }
            }
        }
        else if (column == 1){
            for (int k = 0; k < 15; k++){
                if(s.charAt(k) != '_') {
                    best[k].setText(s.charAt(k) + "");
                    best[k].setBackground(Color.pink);
                }
                else{
                    best[k].setBackground(Color.ORANGE);
                    best[k].setText("");
                }
            }
        }
        else{
            for (int l = 0; l <15; l++){
                if(s.charAt(l) != '_'){
                    worst[l].setText(s.charAt(l) +"");
                    worst[l].setBackground(Color.PINK);
                }
                else{
                    worst[l].setBackground(Color.ORANGE);
                    worst[l].setText("");
                }
            }
        }
    }

//    public static void ResetMemorySpace(){
//        for (int k = 0; k < 15; k++) {
//            first[k].setText("");
//            first[k].setBackground(Color.ORANGE);
//
//            best[k].setText("");
//            best[k].setBackground(Color.ORANGE);
//
//            worst[k].setText("");
//            worst[k].setBackground(Color.ORANGE);
//        }
//    }


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
