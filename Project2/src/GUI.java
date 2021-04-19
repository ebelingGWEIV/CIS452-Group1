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

    static JTextArea upNext;
    static JTextArea timeField;

    static int clock = 0;

    static JTextField first[];
    static JTextField worst[];
    static JTextField best[];

    static Color c1 = new Color(0xF15555);
    static Color c2 = new Color(243, 129, 53);
    static Color c3 = new Color(0xF8CA3B);
    static Color c4 = new Color(0xE8EE2E);
    static Color c5 = new Color(0xA7FF4F);
    static Color c6 = new Color(0x42FF55);
    static Color c7 = new Color(0x40FFAC);
    static Color c8 = new Color(0x4DDDFF);
    static Color c9 = new Color(0x6F8EFD);
    static Color c10 = new Color(0x9345FF);

    static Color[] colors = {c1, c2, c3, c4, c5, c6, c7, c8, c9, c10};


    public Method GetUpdateMethod() throws NoSuchMethodException {
        Method m = GUI.class.getDeclaredMethod("UpdateMemorySpace", int.class, String.class);
        return m;
    }

    public Method GetTimeMethod() throws NoSuchMethodException {
        Method m = GUI.class.getDeclaredMethod("UpdateTime");
        return m;
    }

    public Method GetNext() throws NoSuchMethodException{
        Method m = GUI.class.getDeclaredMethod("UpdateNext", int.class, int.class);
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
        f.setHorizontalAlignment(JTextField.CENTER);
        JTextField b = new JTextField("Best Fit");
        b.setHorizontalAlignment(JTextField.CENTER);
        JTextField w = new JTextField("Worst Fit");
        w.setHorizontalAlignment(JTextField.CENTER);
        JTextField name = new JTextField("CIS 452 Project Two");
        name.setEditable(false);

        for (int i = 0; i < 15; i++) {
            first[i] = new JTextField("");
            first[i].setColumns(5);
            first[i].setHorizontalAlignment(JTextField.CENTER);
            first[i].setBackground(Color.white);

            best[i] = new JTextField("");
            best[i].setColumns(5);
            best[i].setHorizontalAlignment(JTextField.CENTER);
            best[i].setBackground(Color.white);

            worst[i] = new JTextField("");
            worst[i].setColumns(5);
            worst[i].setHorizontalAlignment(JTextField.CENTER);
            worst[i].setBackground(Color.white);

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
        String s = memSpace;
        String s2 = "";
        s = s.substring(1, s.length()-1);

        if (column == 0){
            for (int j = 0; j < 15; j++){
                if(s.charAt(j) != '_') {
                    first[j].setText(s.charAt(j) + "");
                    int a = Character.getNumericValue(s.charAt(j));
                    first[j].setBackground(colors[a]);
                }
                else{
                    first[j].setBackground(Color.white);
                    first[j].setText("");
                }
            }
        }
        else if (column == 1){
            for (int k = 0; k < 15; k++){
                if(s.charAt(k) != '_') {
                    best[k].setText(s.charAt(k) + "");
                    int a = Character.getNumericValue(s.charAt(k));
                    best[k].setBackground(colors[a]);
                }
                else{
                    best[k].setBackground(Color.white);
                    best[k].setText("");
                }
            }
        }
        else{
            for (int l = 0; l <15; l++){
                if(s.charAt(l) != '_'){
                    worst[l].setText(s.charAt(l) +"");
                    int a = Character.getNumericValue(s.charAt(l));
                    worst[l].setBackground(colors[a]);
                }
                else{
                    worst[l].setBackground(Color.white);
                    worst[l].setText("");
                }
            }
        }
    }

    public static void UpdateTime(){
        clock++;
        timeField.setText("Time Units Passed: " + clock);
    }

    // Used for tracking UpdateNext. If java had static local variables they wouldn't be needed.
    private static int updateCounter = 0;
    private static int firstSize;
    private static int bestSize;
    private static int worstSize;

    public static void UpdateNext(int column, int size){
        updateCounter++;

        switch(column){
            case 0:
                firstSize = size;
                break;
            case 1:
                bestSize = size;
                break;
            case 2:
                worstSize = size;
                break;
        }

        if(updateCounter % 3 == 0)
        upNext.setText("Next Process Size to be Allocated: \n"
                + "First: " + firstSize + "\n"
                + "Best: " + bestSize + "\n"
                + "Worst: " + worstSize + '\n');
    }
}
