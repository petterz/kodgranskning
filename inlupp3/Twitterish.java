import java.io.*;
import java.net.*;
import java.util.*;

/**
 * Main class for the startup of Twitterish requiring server id and port number 
 */
public class Twitterish {
    public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("Usage: java Twitterish <server_ip> <port_number>");
        } else {
            try {
                new Client(args[0], Integer.parseInt(args[1])).start();
            } catch (Exception e) {
                e.printStackTrace();
                System.err.println("Something went wrong. Please debug this error.");
            }
        }
    }

    /**
     * Class for the users client holding methods for communicating with the server
     * Holds a list of known users in the server the clients message feed and user info
     */
    
    // This is a nested class, we will go into this later in the course.
    // For now, think of this as a class which is only usable by the Server.
    private static class Client {
        private Account loggedInUser;
        private Set<Account> knownUsers = new TreeSet<Account>();
        private Feed feed;

        private ObjectOutputStream out;
        private ObjectInputStream in;

        private String serverIp;
        private String myIp;
        private int port;

        /**
         * Constructs a client with the given IP and port with an empty feed
         * @param serverIp the server IP
         * @param port the port number
         */
        
        public Client(String serverIp, int port) {
            this.serverIp = serverIp;
            this.port = port;
            this.feed=new Feed();
        }

        /**
         * Adds a new account to the list of known users
         * @param account the account that should be added to the list of known users
         */
        
        private void newAccount(Account account) {
            this.knownUsers.add(account);
        }

        /**
         * Adds a new post to the clients feed
         * @param post the new post that should be added
         */
        
        private void newPost(Post post) {
                feed.addPost(post);
        }

        /**
         * Sends a message to the server holding the action that should be executed
         * @param o holds an object that represents the action
         */
        
        // This is the code that sends a message to the server.
        // You should not need to touch this code.
        private void sendMessage(Object o) {
            try {
                this.outgoing.reset();
                this.outgoing.writeObject(o);
                this.outgoing.flush();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }
        /**
         * Recieves a message from the server holding the key for the class that should be executed
         * and prints the name of the class, normally expecting a syncResponse
         * @return the name of the class that should be executed
         */
        
        // This is the code that receives a message to the server.
        // You should not need to touch this code.
        private Object receiveMessage() {
            try {
                Object o = this.incoming.readObject();
                System.out.printf("Received %s message\n",  o == null ? "<null>" : o.getClass().toString());
                return o;
            } catch (IOException ioe) {
                ioe.printStackTrace();
            } catch (ClassNotFoundException cnfe) {
                cnfe.printStackTrace();
            }
            return null;
        }

        /**
         * Takes a message from the user with keyboard input and sends the message to the server in the form of an object of the type PostMessage
         */
        
        private void postMessage() {
            System.out.println("Write your message on a single line: ");

            String msg = System.console().readLine();
            sendMessage(new PostMessage(msg));

            System.out.println("Message sent");
        }

        /**
         * Prints out the given list of accounts in a row by row format giving each element a number that the user then can choose form
         * @param choices the list of Accounts that we want to select from
         */
        
        private void printEnumeratedChoices(Account[] choices) {
            for (int i = 0; i < choices.length; ++i) {
                System.out.println(i + "\t" + choices[i].getName());
            }
        }

        /**
         * Adds a user as a friend by showing a enumerated list of all known users
         * Sends a message to the server holding the friend that should be added
         * Prints the added friends name       
         */
        
        private void addFriend() {
            if (this.knownUsers.size() == 0) {
                System.out.println("You seem to be alone in the universe, at this moment.");
                return;
            }

            System.out.println("Who to befriend?");

            Account[] knownUsers = (Account[]) this.knownUsers.toArray(new Account[0]);
            Arrays.sort(knownUsers);
            printEnumeratedChoices(knownUsers);

            String choiceString = System.console().readLine();
            int choice = Integer.parseInt(choiceString);
            Account friend = knownUsers[choice];

            sendMessage(new AddFriend(friend));
            this.loggedInUser.addFriend(friend);

            System.out.println("Befriended " + friend.getName());
        }

          /**
         * Removes a user as a friend by showing a enumerated list of all friends
         * Sends a message to the server holding the friend that should be removed
         * Prints the removed friends name       
         */
        
        private void removeFriend() {
            if (this.loggedInUser.hasFriends() == false) {
                System.out.println("You don't have anyone to unfriend. Try to make a few friends first.");
                return;
            }

            System.out.println("Who to unfriend?");
            Account[] friends = this.loggedInUser.getFriends();
            this.printEnumeratedChoices(friends);

            String choiceString = System.console().readLine();
            int choice = Integer.parseInt(choiceString);
            Account friend = friends[choice];

            sendMessage(new RemoveFriend(friend));
            this.loggedInUser.removeFriend(friend);

            System.out.println("Unfriended " + friend.getName());
        }

          /**
         * Adds a user to the ignored friends list by showing a enumerated list of all friends
         * Prints the ignored friend       
         */
        
        private void ignoreFriend() {
            if (this.loggedInUser.hasFriends() == false) {
                System.out.println("You don't have anyone to ignore. Try to make a few friends first.");
                return;
            }

            System.out.println("Who to ignore?");
            Account[] friends = this.loggedInUser.getFriends();
            this.printEnumeratedChoices(friends);

            String choiceString = System.console().readLine();
            int choice = Integer.parseInt(choiceString);
            Account friend = friends[choice];

            this.loggedInUser.ignoreFriend(friend);

            System.out.println("Ignored " + friend.getName());
        }

            /**
         * Removes a user from the ignored friends list by showing a enumerated list of all ignored friends
         * Prints the unignored friend       
         */
        
        private void unIgnoreFriend() {
            Account[] friends=this.loggedInUser.getIgnoredFriends();
            this.printEnumeratedChoices(friends);
            String choiceString = System.console().readLine();
            int choice = Integer.parseInt(choiceString);
            Account friend = friends[choice];
            this.loggedInUser.unIgnoreFriend(friend);
        }

        /**
         * Logging out the open client and sends a message to the server that the user now is offline
         */
        
        private void quit() {
            System.out.println("Logging out...");
            this.sendMessage(new Logout(this.loggedInUser));
        }

        /**
         * Prints out the clients feed ignoring posts from ignored users
         */
        
        private void updateMessagesOnScreen() {
            this.feed.printFeed(this.loggedInUser);
        }

        /**
         * Gives user a chance to edit all account details except userId since this is used to recognize which account that was changed
         * Sends a message to the server holding the new and old account details if the password stated as old was correct
         */
        
        private void editAccount() {
            System.out.print("Enter your password: ");
            String password = new String(System.console().readPassword());

            if (password.equals(this.loggedInUser.getPassword())) {
                UpdateAccount A=new UpdateAccount();
                A.AddOldAccount(this.loggedInUser);  
                System.out.print("Update your password: ");
                this.loggedInUser.setPassword(String.valueOf(System.console().readPassword()));

                System.out.print("Enter your user name: ");
                this.loggedInUser.setName(System.console().readLine());
                A.AddNewAccount(this.loggedInUser);
                this.sendMessage(A);
   
            } else {
                System.out.println("Wrong password!");
            }
        }

        /**
         * Prints the friend list for the logged in user
         */
        
        private void listFriends() {
            if (this.loggedInUser.hasFriends()) {
                Account[] friends = this.loggedInUser.getFriends();
                this.printEnumeratedChoices(friends);
            } else {
                System.out.println("Sorry, but you don't seem to have any friends.");
            }
        }

        /**
         * Gets the current state of logged in users and the new posts from the server as an object of the type syncResponse
         * checks if anyone has changed name since the last sync if yes then change the name of that user 
         * adds the new posts to the clients feed
         */
        
        private void syncWithServer() {
            this.sendMessage(new SyncRequest());
            Object o = this.receiveMessage();
            if (o instanceof SyncResponse) {
                this.knownUsers.addAll(((SyncResponse) o).getUsers());
              
                Iterator<Account> it;
                for (it=((SyncResponse) o).getUsers().iterator(); it.hasNext();) {
                    Account next = it.next();

                    Iterator<Account> it2;
                    for (it2=this.knownUsers.iterator(); it2.hasNext();) {
                        Account next2=it2.next();

                        if (next2.getUserId().equals(next.getUserId())) {
                            next2.setName(next.getName());

                            
                        }
                    }
                }
           
                 
                for (Post p : ((SyncResponse)o).getPosts())
                    this.newPost(p);

            } else {
                System.out.println("Error: expected sync response, got " + o.getClass());
            }
        }

        private ObjectOutputStream outgoing;
        private ObjectInputStream incoming;

        /**
         * Gets the details for the account that wats to login and sends a message of type login with the account details to the server
         * If the account exists then the account is logged in if the passwords match and sent back to the client as the loggedInUser
         * Otherwise a new account is created and sent back to the client as the loggedInUser
         */
        
        private void loginOrCreateUser() throws IOException, UnknownHostException {
            Socket socket = new Socket(this.serverIp, port);
            this.outgoing = new ObjectOutputStream(socket.getOutputStream());

            System.out.print("Enter your user id (email address): ");
            String userid = System.console().readLine();
            System.out.print("Set your password: ");
            String password = new String(System.console().readPassword());
            System.out.print("Enter your user name: ");
            String name = System.console().readLine();

            assert(userid.length() > 0);
            assert(password.length() > 0);
            assert(name.length() > 0);

            System.out.println("Logging in new user " + userid + "...");
            outgoing.writeObject(new Login(new Account(userid, password, name)));

            this.outgoing = outgoing;
            incoming = new ObjectInputStream(socket.getInputStream());
            Account a = (Account) receiveMessage();
            this.loggedInUser = a;
        }

        /**
         * Writes "Twitterish"
         */

        private void displaySplashScreen() {
            System.out.println("");
            System.out.println("");
            System.out.println("  _______       _ _   _            _     _      ");
            System.out.println(" |__   __|     (_) | | |          (_)   | |     ");
            System.out.println("    | |_      ___| |_| |_ ___ _ __ _ ___| |__   ");
            System.out.println("    | \\ \\ /\\ / / | __| __/ _ \\ '__| / __| '_ \\  ");
            System.out.println("    | |\\ V  V /| | |_| ||  __/ |  | \\__ \\ | | | ");
            System.out.println("    |_| \\_/\\_/ |_|\\__|\\__\\___|_|  |_|___/_| |_| ");
            System.out.println("");
        }

        /**
         * Displays a list of the functions in twitterish and let's the user choose which one to execute
         * Compares the users answer to the different cases and executes the correct method
         * @return true aslong as the client isn't shut down
         * @return false if client is shut down
         */
        
        private boolean action() {
            System.out.println("Actions:");
            System.out.print("[P]ost message     |  ");
            System.out.print("[S]ync with server |  ");
            System.out.print("[A]dd friend       |  ");
            System.out.print("[R]emove friend    |  ");
            System.out.println();
            System.out.print("[I]gnore friend    |  ");
            System.out.print("Unignore [f]riend    |  ");
            System.out.print("[L]ist friends     |  ");
            System.out.print("[E]dit account     |  ");
            System.out.print("[U]pdate           |  ");
            System.out.print("[Q]uit");
            System.out.println();

            String input = System.console().readLine().toLowerCase();

            if (input.length() != 1) {
                System.out.println("Invalid input");
                return true;
            }

            switch (input.charAt(0)) {
            case 'p':
                this.postMessage();
                return true;
            case 's':
                this.syncWithServer();
                return true;
            case 'a':
                this.addFriend();
                return true;
            case 'r':
                this.removeFriend();
                return true;
            case 'i':
                this.ignoreFriend();
                return true;
            case 'f':
                this.unIgnoreFriend();
                return true;
            case 'e':
                this.editAccount();
                return true;
            case 'l':
                this.listFriends();
                return true;
            case 'u':
                this.updateMessagesOnScreen();   
                return true;
            case 'q':
                this.quit();
                return false;
            }

            return true;
        }

        /**
         * Initiatezzzz the clients welcome screen, includes exceptions and starts the action loop
         */
        
        public void start() throws IOException, UnknownHostException {
            this.displaySplashScreen();
            this.loginOrCreateUser();
            while (this.action())
                ;
        }
    }
}
