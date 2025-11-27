package Systems.General;

public class Ability {
    private String name;
    private String shortName;
    private int value;
    private int modifier;

    public Ability(String name, String shortName, int value) {
        this.name = name;
        this.shortName = shortName;
        this.value = value;
        this.modifier = (value/2)-5;
    }

    public Ability(String name, int value, int modifier) {
        this.name = name;
        this.shortName = name.substring(0, Math.min(name.length(), 3));;
        this.value = value;
        this.modifier = (value/2)-5;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getShortName() {
        return shortName;
    }

    public void setShortName(String shortName) {
        this.shortName = shortName;
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
        this.modifier = (value/2)-5;
    }

    public int getModifier() {
        return modifier;
    }
}
