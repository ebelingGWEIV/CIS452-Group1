/* This is a simple Java program.
   FileName : "HelloWorld.java". */

class HelloWorld
{
    // Your program begins with a call to main().
    // Prints "Hello, World" to the terminal window.
    public static void main(String[] args) throws InterruptedException {
        System.out.println("Hello, World");

        try {
            var myProc = new TimedProcess(20);
        }
        catch (Exception ex){
            ex.printStackTrace();
            System.out.println("couldn't start process");
    }
        Thread.sleep(25);

    }
}