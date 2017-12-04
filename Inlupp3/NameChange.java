import java.io.Serializable;

public class NameChange implements Serializable {
    private Account oldacc;
    private Account newacc;

    public NameChange(Account oldname, Account newname) {
        this.oldacc = oldname;
        this.newacc = newname;
    }

    public Account getOldName() {
        return this.oldacc;
    }

    public Account getNewName() {
        return this.newacc;
    }
}
