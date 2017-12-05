import java.util.*;                    
import java.io.*;
import java.net.*;


/**
 * class for the server part of the part of the twitterish program that allows clients to
 *communicate with each other. 
 */
public class Server {
    private Set<Account> knownUsers = new TreeSet<Account>();
    private List<Post> posts = new LinkedList<Post>();


    /**
     * main function to initiate the server
     */
    
    public static void main(String[] args) {
        try {
            ServerSocket socket = new ServerSocket(args.length > 0 ? Integer.parseInt(args[0]) : 8080);
            Server server = new Server();

            while (true) {
                System.out.println("!! Server listening for connections: " + socket.getInetAddress() + ":" + socket.getLocalPort());
                Socket clientConnection = socket.accept();
                System.out.println("!! Server got a connection from: " + clientConnection.getInetAddress() + ":" + clientConnection.getPort());
                try {
                    ClientProxy.attemptEstablishConnection(clientConnection, server);
                } catch (RuntimeException e) {
                    System.err.println(e.getMessage());
                }
            }
        } catch (IOException ioe) {
            ioe.printStackTrace();
        } catch (ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        }
    }

    
    /**
     * Allows you to access an account by using it's userId
     *@param userId the userId for an account
     *@return Account if an Account matches the given userId, otherwise if no account is found it returns null
     */
    
    public Account getAccountFor(String userId) {
        for (Account a : this.knownUsers)
            if (a.getUserId().equals(userId)) return a;

        return null;
    }

    
    /**
     * Adds a given account to knownUsers
     * @param a is an account
     */
    
    public synchronized void addAccount(Account a) {
        this.knownUsers.add(a);
    }

    
    /**
   *removes an account from knownUsers
   *@param a is an account
   */
    
    public synchronized void removeAccount(Account a) {
        this.knownUsers.remove(a);
    }

    
    /**
     *Gives the requester all accounts from the server
     *@return Set<Account> a set of all accounts in knownUsers
     */
    
    public synchronized Set<Account> getAccounts() {
        return new TreeSet<Account>(this.knownUsers);
    }

    
    /**
     * gives the requester all posts from the server
     * @return List<Post> a list of all posts
     */
    
    public synchronized List<Post> getPosts() {
        return new ArrayList<Post>(this.posts);
    }
    

    /**
     * Adds a post to posts
     * @param p is a Post
     */
    
    public synchronized void addPost(Post p) {
        this.posts.add(p);
    }


    /**
     * gets all the posts from the requesting users friends.
     *@param a is an Account
     *@return List<Post> a list of post
     */
    
    public synchronized List<Post> getNewFriendPosts(Account a)
    {
        List<Post>result = new ArrayList<Post>();
        for (Post p : this.getNewPosts(a)){
            if (a.isFriendsWith(p.getPoster())){
                result.add(p);
            }
        } 
        
        return result;
        }
    /**
     * gives back all posts that have been posted since last sync
     *@param a is an Account
     *@return List<Post> a list of posts
     */
    public synchronized List<Post> getNewPosts(Account a){
        int postsLastSync=a.getPostAtLastSync();
        a.setPostAtLastSync(this.posts.size());
        return new ArrayList<Post>(this.posts.subList(postsLastSync, this.posts.size()));
    }

    /**
     * a subclass of server that holding methods to communicate with clients
     */
    
    static class ClientProxy extends Thread {
        private Account account;
        private Socket socket;
        private Server server;
        private ObjectOutputStream outgoing;
        private ObjectInputStream incoming;

        private ClientProxy(Account account, Socket socket, Server server, ObjectInputStream incoming) throws IOException {
            this.account = account;
            this.server  = server;
            this.socket  = socket;
            this.incoming = incoming;
            this.outgoing = new ObjectOutputStream(socket.getOutputStream());
            System.out.println("<< Account");
            this.outgoing.writeObject(account);
            this.outgoing.flush();
        }



        /**
         *attempts to establish connection between the respective client and the server, with the
         * details from the socket, checks if the user trying to connect already exists.  
         *@param server is the server.
         *@param socket is a Socket, that contains details such as server ip and portnumber
         */ 
        
        public static void attemptEstablishConnection(Socket socket, Server server) throws IOException, ClassNotFoundException {
            ObjectInputStream incoming = new ObjectInputStream(socket.getInputStream());
            Object handShake = incoming.readObject();

            if (handShake instanceof Login) {
                Account account = ((Login) handShake).getAccount();
                Account knownAccount = server.getAccountFor(account.getUserId());

                if (knownAccount == null) {
                    server.addAccount(account);
                    new ClientProxy(account, socket, server, incoming).start();
                } else {
                    if (knownAccount.getPassword().equals(account.getPassword()) == false) throw new RuntimeException("Wrong password");
                    new ClientProxy(knownAccount, socket, server, incoming).start();
                }
            } else {
                System.err.println("!! Bad connection attempt from: " + socket.getInetAddress() + ":" + socket.getPort());
            }
        }
        private int globalPostIdCounter = 0;
        
        /**
         *gives each post an individual unique id. 
         *@return the unique id of the post
         */
        
        private synchronized int getUniqueGlobalPostId() {
            return ++this.globalPostIdCounter;
        }


        /**
         * logs the user off from the server
         * @param a is an Account
         */
        
        private void logout(Account a) {
            this.server.removeAccount(a);
            System.out.println("!! " + a.getUserId() + " left the building");
            try {
                this.outgoing.close();
                this.incoming.close();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }


        /**
         * allows the user to post a new message to the server
         *@param msg is a string, and is the message to be posted to the server
         */
        
        private void postMessage(String msg) {
            this.server.addPost(new Post(this.getUniqueGlobalPostId(), this.account, msg));
        }


        /**
         * adds a friend to the users friendslist
         * @param a is the account to be added to the users friendslist
         */
        
        private void addFriend(Account a) {
            this.account.addFriend(a);
            a.addFriend(this.account);
        }



        /**
         * removes a friend from the accounts friendslist
         * @param a is the friend to be removed
         */
        
        private void removeFriend(Account a) {
            this.account.removeFriend(a);
            a.removeFriend(this.account);
        }

        
        /**
         *Allows the user the change it's name
         *@param old is the the current user
         *@param neu is what the account should be updated to
         */
        
        private void updateAccount(Account old, Account neu) {
            Account A= server.getAccountFor(old.getUserId());
            A.setName(neu.getName()); 
        }


        /**
         * send a synqrequest to the the client with the post list and account list
         */

        private void sync() {  
            try {
                this.outgoing.reset();
                System.out.println("<< SyncResponse");
                this.outgoing.writeObject (new SyncResponse(new HashSet<Account>(this.server.getAccounts()),
                                              new LinkedList<Post>(this.server.getNewFriendPosts(this.account))));
                this.outgoing.flush();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }      
            }



        /**
         * is an infinte loop that waits for an incomming message with an object, depending
         * on what type of object different functions will be applied to it.
         *
         */
        public void run() {
            try {
                while (true) {
                    Object o = this.incoming.readObject();
                    System.err.println(">> Received: " + o.getClass().getName());
                    // o instanceof Account checks if o is an account
                    // (Account) o type casts o into an Account so that it can be used as one
                    if (o instanceof Account) {
                      this.updateAccount(this.account, (Account) o);
                    } else if (o instanceof UpdateAccount) {
                        this.updateAccount((((UpdateAccount) o).getOldAccount()),(((UpdateAccount) o).getNewAccount()));
                    } else if (o instanceof PostMessage) {
                        this.postMessage(((PostMessage) o).getMsg());
                    } else if (o instanceof AddFriend) {
                        this.addFriend(((AddFriend) o).getFriend());
                    } else if (o instanceof RemoveFriend) {
                        this.removeFriend(((RemoveFriend) o).getFriend());
                    } else if (o instanceof SyncRequest) {
                        this.sync();
                    } else if (o instanceof Logout) {
                        this.logout(((Logout) o).getAccount());
                        return;
                    }
                }
            } catch (Exception e) {
                // BAD Practise. Never catch "Exception"s. Too general.
                e.printStackTrace();
            }
        }
    }
}
