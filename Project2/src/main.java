

class HelloWorld
{
    // Your program begins with a call to main().
    // Prints "Hello, World" to the terminal window.
    public static void main(String[] args) throws InterruptedException, NoSuchMethodException {

        var myGUI = new GUI();
        var updateMethod = myGUI.GetUpdateMethod();
        var updateTime = myGUI.GetTimeMethod();
        var updateNext = myGUI.GetNext();

        myGUI.displayGUI();

        try {
            var controller = new MemoryController(15, updateMethod, updateTime, updateNext);
            controller.Run();
        }
        catch (Exception ex){
            ex.printStackTrace();
            System.out.println("couldn't start process");
    }
        Thread.sleep(25);

    }
}