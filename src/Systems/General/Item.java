package Systems.General;

public class Item {
    private String name;
    private int price;
    private int rarity;

    public Item(String name, int price, int rarity) {
        this.name = name;
        this.price = price;
        this.rarity = rarity;
    }

    public String getName() {
        return name;
    }
    public int getPrice() {
        return price;
    }
    public int getRarity() {
        return rarity;
    }

    public void setName(String name) {
        this.name = name;
    }
    public void setPrice(int price) {
        this.price = price;
    }
    public void setRarity(int rarity) {
        this.rarity = rarity;
    }
}
