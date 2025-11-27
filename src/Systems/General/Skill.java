package Systems.General;

public class Skill {
    private String name;
    private Ability ability;
    private int modifier;

    Skill(String name, Ability ability, int modifier) {
        this.name = name;
        this.ability = ability;
        this.modifier = modifier;
    }
    Skill(String name, Ability ability) {
        this.name = name;
        this.ability = ability;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getModifier() {
        return modifier;
    }

    public void setModifier(int modifier) {
        this.modifier = modifier;
    }

    public Ability getAbility() {
        return ability;
    }

    public void setAbility(Ability ability) {
        this.ability = ability;
    }
}
