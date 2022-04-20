#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <emscripten/bind.h>
using namespace emscripten;
using std::vector;
using std::string;
using std::swap;

// Restaurant names come from https://namesbee.com/restaurant-names/

class Restaurant
{
private:
public:

    int rating;
    float distance;
    string cuisine;
    string name;
    float simScore;

};

// passes in the data set, the searched values, and the preferred way of sorting (distanceOrRating is true if distance is preferred)
void setSimScores(vector<Restaurant>& dataSet, float prefDistance, int prefRating, string prefCuisine, bool distanceOrRating)
{
    // sets sim scores with a preference towards distance
    if(distanceOrRating)
    {
        for(int i = 0; i < dataSet.size(); i++)
        {
            int simScoreCuisine = -1;
            int ratingFit = -1;
            if(dataSet[i].cuisine == prefCuisine | prefCuisine == "Any")
            {
                simScoreCuisine = 1;
            }
            if(dataSet[i].rating >= prefRating | prefRating == 0) {
                ratingFit = 1;
            }
            dataSet[i].simScore = (.75 * ((prefDistance - dataSet[i].distance) / prefDistance)) + ratingFit + simScoreCuisine;
        }
    }
        // sets sim scores with a preference towards rating
    else
    {
        for(int i = 0; i < dataSet.size(); i++)
        {
            int simScoreCuisine = -1;
            int disFit = -1;
            if(dataSet[i].cuisine == prefCuisine | prefCuisine == "Any")
            {
                simScoreCuisine = 1;
            }
            if (dataSet[i].distance <= prefDistance | prefDistance == 10000) {
                disFit = 1;
            }
            dataSet[i].simScore = disFit + (.75 * ((dataSet[i].rating - prefRating) / 5.0)) + (1 * simScoreCuisine);
        }
    }
}

int partition(vector<Restaurant> &array, int low, int high)
{
    float pivot = array[low].simScore;
    int up = low - 1;
    int down = high + 1;
    while(true)
    {
        do {
            up++;
        } while (array[up].simScore > pivot);

        do {
            down--;
        } while (array[down].simScore < pivot);

        if (up >= down)
            return down;

        swap(array[up], array[down]);
    }
}

// randomize pivot
int partition_r(vector<Restaurant> &array, int low, int high) {
    srand(time(NULL));
    int random = low + rand() % (high-low);
    swap(array[random], array[low]);
    return partition(array, low, high);
}

// call with the dataset, 0, and the size - 1
void QuickSort(vector<Restaurant> &array, int low, int high)
{
    if(low >= high)
    {
        return;
    }
    if(low < high)
    {
        int pivot = partition_r(array, low, high);
        QuickSort(array, low, pivot);
        QuickSort(array, pivot + 1, high);
    }
}

void Merge(std::vector<Restaurant>& array, int start, int mid, int end){
    int leftCount = mid - start + 1;
    int rightCount = end - mid;
    std::vector<Restaurant> left;
    std::vector<Restaurant> right;



    //Left and right subarrays
    for(int i = 0; i < leftCount; i++){
        left.push_back(array[start + i]);
    }
    for(int i = 0; i < rightCount; i++){
        right.push_back(array[mid + 1 +i]);
    }

    int leftIndex = 0;
    int rightIndex = 0;
    int arrIndex = start;
    //Initial merge instructions
    while(leftIndex < leftCount && rightIndex < rightCount){
        if(left[leftIndex].simScore > right[rightIndex].simScore){
            swap(array[arrIndex], left[leftIndex]);
            leftIndex++;
        }
        else{
            swap(array[arrIndex], right[rightIndex]);
            rightIndex++;
        }
        arrIndex++;
    }
    //Copy leftover values
    while(leftIndex < leftCount){
        swap(array[arrIndex], left[leftIndex]);
        leftIndex++;
        arrIndex++;
    }
    while(rightIndex < rightCount){
        swap(array[arrIndex], right[rightIndex]);
        rightIndex++;
        arrIndex++;
    }
}

void MergeSort(std::vector<Restaurant>& array, int start, int end){
    if(start < end) {
        int mid = (end + start) / 2;
        MergeSort(array, start, mid);
        MergeSort(array, mid + 1, end);
        Merge(array, start, mid, end);
    }
}

vector<Restaurant> makeData(int numbOfRestaurant)
    {
        vector<Restaurant> dataSet;
        vector<string> cuisineTypes {"American", "French", "Mexican", "Italian", "Japanese", "Indian", "Chinese","Ethiopian", "Lebanese", "Kosher", "Thai", "Spanish", "Cuban"};
        vector<string> restaurantNames {"Benno Restaurant", "Benno Restaurant", "Big Moe's Diner" , "Pizza Italian Heart", "Smokey's Texas Grill",
                                        "USA Bakery", "Papa John's", "Karachi Silver Spoon", "Pizzeria Cafe", "Jockey", "Oriole", "Zahav",
                                        "Gramercy Tavern", "Le Bernardin", "Alinea", "Daniel", "Manresa", "Lahaina Grill", "Canlis", "Le Coucou",
                                        "Le Diplomate", "The Modern", "Momofuku Ko", "Vernick Food & Drink", "Monteverde Restaurant & Pastificio",
                                        "Girl & The Goat", "Next Restaurant", "Eleven Madison Park", "Marea", "Boka", "Gotham", "Benu", "Saison",
                                        "Talula's Garden", "Blue Hill", "Charleston", "Le Pigeon", "Highlands Bar & Grill", "Vetri Cucina",
                                        "Gabriel Kreuther","Del Posto", "Coquine", "Kokkari Estiatorio", "Quince", "Acquerello", "Blue Smoke",
                                        "Bohemian","Carmine's Italian Restaurant","Club A Steakhouse", "Your Aesthetic","Alphabet Eatery",
                                        "Let's Ambush","Anna Bella", "Arrow Spoon", "The Taste and Beyond", "Blind Faith","Blue Valley",
                                        "Breakfast at any Time", "Cast Diners", "Cheesy Love", "Chef First", "The Eclectic Taste","Chef Parade",
                                        "Chicken and Spices", "Chili Flora", "Chipotle Meal", "Classic Corn in", "Crazy Cut","Crazy Grill",
                                        "The Lakhani", "The Hebbet", "Seven Spices", "Hot Gear", "Hollo Follo", "Hill Crest","Heart Beats food",
                                        "Harley Food Center", "Grebb", "Brewhouse", "Dine Fine", "Perfect Place","Pointe Restaurant", "The Spice",
                                        "Gourmet Meal", "Tavern", "Fresh Ingredients", "Umami Burger","Amsterdam", "Finest Dining", "Tofino",
                                        "Pasta Beach", "French Gourmet", "Oasis Cafe", "Mediterranean Seafood", "Sensory Experience", "Fish and Chips",
                                        "Phuc Noodle", "Captiva Island", "Perfect Dish", "Pinch Kitchen", "Pita Pan", "Plane Grill", "Planet of the Grapes",
                                        "Plumed Horse", "PM Fish & Steak House", "Poke Life", "Project Juice", "Quick Eats", "Rainforest Cafe",
                                        "Ready Restaurants", "Red Dragon", "Red Tablecloth", "Revelry Bistro", "Rice House", "Rich Table", "Rocco's Cafe",
                                        "Rock and Roll cafe", "Rolls", "Salted Grill", "Fred's Tacos Corner", "Fatty Fingers", "Eat Bite", "Desi Eatery",
                                        "Dagny's Delight", "Curry Out", "Crew Cafe", "The Elephant and the Mouse", "The Lazy Lemur", "Naan Better", "The Hungrella",
                                        "Hurry Curry", "Mad Munch", "MadWings", "Chophouse", "Porridge Cafe", "Homemade Bagels", "Strip Steak", "Burger King",
                                        "Steam Plant", "Pho Shizzle", "Salty Squid", "Sea Spice", "Shaker + Spear", "Skillet Counter", "Sloppy Eats",
                                        "Soul Food", "Spice Villa", "Spicy Dragon", "Street Delights", "Sugar Blast", "Sunrise Cafe", "Taco Mayo",
                                        "Tall Oaks Cafe", "Bones Restaurant", "ATOMIX", "Cafe Monarch", "Charleston Grill", "Restaurant Gordon Ramsay",
                                        "Spoon and Stable", "Melisse Restaurant", "Jean-Georges", "Rose's Luxury", "Frasca Food and Wine", "FIG",
                                        "The Old Fashioned", "The Table", "Mama's Fish House", "COI", "GW Fins", "Open Zest", "Palm Crown",
                                        "The New Montana Restaurant", "Sharp Knives", "Snack Bar Express", "Street Stuff", "Sweet Munchies",
                                        "Suede Dinner", "Vintage Burgers", "Wave's", "West Coast Chef", "Tribal Fiesta", "Chipotle Mexican Grill",
                                        "Banana Leaf", "Spear Smoque", "Thai Me Up", "Thai The Knot", "The Back Room", "The Breakfast Story",
                                        "The Cafe Baraco", "The Capital Grille", "The Chef in the Hat","The Dinning Room", "The Fishery",
                                        "The Food Place", "The French Gourmet", "The Golden Stool", "Les Nomades", "The Original", "CRUST",
                                        "Gordon Ramsay Hell's Kitchen", "LEON", "Aliada", "Patina Restaurant", "Cliff House", "Lawry's The Prime Rib",
                                        "Bryant Park Grill", "Rudy & Paco Restaurant and Bar", "Gibsons Bar & Steakhouse", "Atelier Crenn",
                                        "Orchids at Palm Court", "The Compound Restaurant", "Masa", "Crisp", "The Aviary", "Tasty Elements",
                                        "Starbelly", "Pier 23", "Lord Stanley", "Gemini", "Catch 35", "Blue Plate", "Burger & Beer Joint",
                                        "CRUST", "Full Moon", "Grubstake", "Hot & Crusty", "A Salt & Battery", "Chart House", "Double Decker",
                                        "Foxsister", "Halls Chophouse", "King and Queen", "Lazy Bear", "Mad for Chicken", "Americano", "Buccan",
                                        "Blue Hill At Stone Barns", "Boston's Restaurant & Sports Bar", "Cured", "Johnny Rockets", "The Root Cafe",
                                        "Lantern", "Handle", "Chin Chin Las Vegas", "Next Door American Eatery","V's Italiano Ristorante",
                                        "S. Egg Brunch Restaurant North Scottsdale", "Eat Restaurant", "The Breakfast Bar", "The Chef",
                                        "La Lucha", "Chimney Park Restaurant & Bar", "Chez Panisse", "Vie Restaurant", "Pizzeria Delfina Mission",
                                        "The Sea Spice", "German Food", "The Ledbury", "Stateside", "The Capital Grille", "The Egg & Us", "The Local Eatery",
                                        "The River Seafood", "Townsend", "Wise Sons", "Zero Restaurant", "Bean Around", "Chewy Balls", "Double Knot",
                                        "Fog Harbor", "Hereford Grill", "Like No Udder", "Munch Box", "North Beach", "Queenstown Public House",
                                        "True Food Kitchen", "The Local House", "Stomach Clinic", "Sundown at Granada", "HG Sply Co.", "Toulouse Knox Street",
                                        "Taverna", "Al Biernat's", "Up On Knox", "Thai Thai Restaurant", "Pecan Lodge", "The Capital Grille",
                                        "The Polo Bar", "The Red Door", "The Rosebud", "The Saddle River Inn", "The Slanted Door", "The Spanish Kitchen",
                                        "Tin Roof", "Top of the Market", "Townsend", "Udupi", "Unique Meals", "Urban Remedy", "Vegans Come Here",
                                        "Vessel Restaurant", "Vietnamese Street Food", "Walton's Restaurant", "Water Grill", "Wok This Way",
                                        "Xin Chao Vietnamese Restaurant", "Your Cook", "Columbia Restaurant Celebration", "CityGrocery",
                                        "The Proper Restaurant & Bar", "Moto", "ROKU", "The Mill Restaurant", "Turn", "Heart Attack Grill",
                                        "Top NotchHamburgers", "Honest Restaurant Lowell", "Proof Restaurant", "Kura Revolving Sushi Bar",
                                        "Di Fara Pizza", "Valor Glencoe", "All Set Restaurant & Bar", "Sister", "Chipotle", "Story Restaurant",
                                        "Thrive", "The Pantry Restaurant", "The Federal", "Tom's Sushi House", "Le Parfait", "Bright", "Sabor",
                                        "Dinner by Heston Blumenthal", "Scully St James's", "Angler","Dobar", "Kinship", "Caviar Bar",
                                        "Fishing with Dynamite", "Conch it Up Soul Food", "California Pizza Kitchen", "BeaverChoice", "Pita Pan",
                                        "Sears Fine Food", "The Bear & The Monarch", "The Patio", "Zareen's", "Sushi Tomi", "Cascal",
                                        "Crepevine Restaurants", "Eureka!", "Paul Martin's America", "Napoletana Pizzeria", "Le Petit Bistro",
                                        "ViVe Sol", "SAJJ Mediterranean", "Ding Tai Fung", "El Mercado Restaurant", "MELT", "Dozens Restaurant",
                                        "BREWED", "The Standard", "Nusr-Et Steakhouse Dallas", "Steve & Cookie's By the Bay", "The Oven",
                                        "Carlos' Bistro", "AVANT Restaurant", "The Taste", "The Peak", "Social", "Nathan's Famous", "The Local",
                                        "Fine Burgers & Drinks", "The Place", "Zona Italian Restaurant", "Start Restaurant", "Sizzler",
                                        "Los Padres Mexican Food", "The Point Restaurant", "The Point Bar & Grill", "A&W", "The Loop Minneapolis",
                                        "Fin Japanese Cuisine", "Around the Corner", "Lawry's Restaurants, Inc.", "Benihana", "Hanna's",
                                        "Casa Bonita", "Curbside Burgers", "The Dallas World Aquarium", "Denny's", "The Roof", "Cinco Mexican Cantina",
                                        "Companion Bakery", "Breakfast Nook", "The Inn at Little Washington", "Local Beer, Patio and Kitchen",
                                        "The Original Mexican Restaurant","St Francis Winery & Vineyards", "The MARK", "Cinemark Hollywood Movies 20",
                                        "US Foods CHEF'STORE", "Darden Restaurants", "All Elite Food", "Alpine Meadow", "At Your Service", "Bacon Babe Burgers",
                                        "Bankers Hill", "Barefoot Bar & Grill", "Basic Kneads Pizza", "BBQ Night", "Beanburgers", "Before the Bistro","Big Bills",
                                        "Big Chef", "Big Mountain", "Bistro","Bite Me Sandwiches", "Bite-Sized", "Blue Collar", "Blue Mermaid", "Boogie bites",
                                        "Bread And Butter", "Breakfast House", "Brick House Stackhouse", "Brothers Railroad Inn", "Burger & Beer Joint",
                                        "Burger UK London", "Burma Love", "Cafe21", "Cafe Coyote", "Cafe Provence", "California Pizza Kitchen", "Cat Heads BBQ",
                                        "Catch of the Day", "Cheesy", "Chewy Balls", "Chicago", "Chilango", "Chops N' Drinks", "Circus", "City View Restaurant",
                                        "Coffee and Cake", "Corner", "Cosmopolitan Restaurant", "Crabby Dick's", "Crown Crest", "Crusty Chicken", "Curated Cuisine",
                                        "Curry", "Daily Grill", "Dedicated Dining", "Delightful Dining", "Detox Kitchen", "Dine Dime", "Dining Delight",
                                        "Dining Room Ready", "Dinner by HestonBlumenthal", "Double Knot", "Drink And Dive", "East Meets West", "Eat Your Heart Out",
                                        "El Charro of Iola", "Emerald Grill", "Everest", "Famous Lunch", "Fiddler's Green", "Fine Dining Delivered", "Food O'clock",
                                        "Firefly Resort", "Five Oceans", "Flame On", "Fog City", "Food Addict", "Fishing with Dynamite", "Food Court", "Food Formula",
                                        "Food Town", "FoodiesUnites", "Foodstuff", "Formal Performance", "Full Belly Belles", "Garage Kitchen + Bar", "Gem Cafe",
                                        "Geronimo", "Golden Era", "Goldfinch Tavern", "Good Morning", "King and Queen", "King Lee's", "Mad Chicken", "Mad for Chicken",
                                        "Fury Kitchen", "Koi's Sushi's", "Last Time", "Le Parfait", "Lettuce Eat", "Lionfish", "Little London", "Long Eel",
                                        "Lord Stanley", "Love Shack Foods", "Lunch Chow", "Malabar", "Marina Kitchen", "Masa Grill Kabob", "Maude", "Meat U There",
                                        "Mess Hall", "Blue Mermaid", "Duke's Seafood", "Hillstone", "Cat Heads BBQ", "Eatmore Fried Chicken", "Great Eastern",
                                        "BarefootBar & Grill", "Little Sheep", "Metro Cafe", "Ocean Star", "Peking Inn", "Single Shot", "Thai Me Up",
                                        "Harbor City", "Metropolitan Grill", "Rice House", "The House", "The Kitchen", "The Mission", "Tin Roof", "Barley Mash",
                                        "Coaster Saloon","Waterfront", "The Old Spaghetti", "Tanta", "Sotto", "Revelry Bistro", "Next", "Kum Den Bar",
                                        "Goldfinch Tavern", "Zum Schweizerhaus", "Spice Bazaar", "Urchig", "Alpenclub", "Stand", "Ski Lodge Engelberg", "McDongalds Borger"};
        for(int i = 0; i < numbOfRestaurant; i++)
        {
            Restaurant newRestaurant = Restaurant();
            newRestaurant.rating = rand() % 6;
            float temp = rand() % 100 ;
            temp = temp / 100;
            newRestaurant.distance = rand() % 49 + temp;
            int cuisineLocation = rand() % cuisineTypes.size();
            newRestaurant.cuisine = cuisineTypes.at(cuisineLocation);
            int nameLocation = rand() % restaurantNames.size();
            newRestaurant.name = restaurantNames.at(nameLocation);
            dataSet.push_back(newRestaurant);
        }
        return dataSet;
    }

EMSCRIPTEN_BINDINGS(my_module) {

    class_<Restaurant>("Restaurant")
        .constructor<>()
        .property("rating",&Restaurant::rating)
        .property("distance", &Restaurant::distance)
        .property("cuisine", &Restaurant::cuisine)
        .property("name", &Restaurant::name)
        .property("simScore", &Restaurant::simScore)
    ;

    function("setSimScores", &setSimScores);
    function("makeData", &makeData);
    function("QuickSort", &QuickSort);
    function("MergeSort", &MergeSort);

    register_vector<Restaurant>("vector<Restaurant>");
    register_vector<string>("vector<string>");
}