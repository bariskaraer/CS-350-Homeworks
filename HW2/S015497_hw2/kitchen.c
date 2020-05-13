/** <Barış Karaer, S015497>
 * This is the kitchen simulation code for OzuRest.
 * French chef and 3 students from gastronomy department are preparing delicious meals in here
 * You need to solve their problems.
**/
#include "meal.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define GLOVE_COUNT 3
#define POT_SIZE 3
#define APPRENTICE_COUNT 3
#define MEALS_TO_PREPARE 4
#define REQUIRED_INGREDIENTS 3

struct meal Menu[4] = {
    {"Menemen", {{"Tomato", 3}, {"Onion", 4}, {"Egg", 1}}, 10},
    {"Chicken Pasta", {{"Pasta", 2}, {"Chicken", 5}, {"Curry", 2}}, 8}, 
    {"Beef Steak", {{"Beef", 7}, {"Pepper", 3}, {"Garlic", 2}}, 13}, 
    {"Ali Nazik", {{"Eggplant", 4}, {"Lamb Meat", 4}, {"Yoghurt", 1}}, 10}
}; 

int exe_done = 0;
int meal_counter = 0;
int meal_ing_number = 0;
int pot_counter = 0;
int pick_meal_counter = 0;

// needed variables

// Define all required mutexes here

pthread_mutex_t glove_mutex[GLOVE_COUNT];
pthread_mutex_t pot_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ingredient_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t eat_mutex;

void put_gloves(int apprentice_id) {
    // Normally used this but dont need it anymore
    //int v;
    //v = *apprentice_id;
    //pthread_mutex_lock(&glove_mutex[v]);
    //pthread_mutex_lock(&glove_mutex[(v+1)%GLOVE_COUNT]);
    printf("Apprentice %d is picking gloves \n", apprentice_id);
    // Implement a mutex lock mechanism for gloves here
    if (apprentice_id == 2) { // 3rd apprentice will use this code
    pthread_mutex_lock(&glove_mutex[0]);
    pthread_mutex_lock(&glove_mutex[apprentice_id]);
    //pthread_mutex_lock(&glove_mutex[0]);  the same thing
    //pthread_mutex_lock(&glove_mutex[v]);
    } else { // other apprentices will use thhis code
    pthread_mutex_lock(&glove_mutex[apprentice_id]);
    pthread_mutex_lock(&glove_mutex[apprentice_id + 1]);
    //pthread_mutex_lock(&glove_mutex[v]);  the same thing
    //pthread_mutex_lock(&glove_mutex[(v+1)%GLOVE_COUNT]);
    }

    

    printf("Apprentice %d has picked gloves\n", apprentice_id);
}

void remove_gloves(int apprentice_id) {
    // Implement a mutex unlock mechanism for gloves here
    //int v;
    //v = *apprentice_id;
    // Normally used this but dont need it anymore

    if (apprentice_id == 2) { // 3rd apprentice will use this code
    pthread_mutex_unlock(&glove_mutex[0]);
    pthread_mutex_unlock(&glove_mutex[apprentice_id]);
    } else {  // other apprentices will use thhis code
    pthread_mutex_unlock(&glove_mutex[apprentice_id]);
    pthread_mutex_unlock(&glove_mutex[apprentice_id + 1]);
    }
 
    //pthread_mutex_unlock(&glove_mutex[v]);
    //pthread_mutex_unlock(&glove_mutex[(v+1)%GLOVE_COUNT]);
    // same stuff with put gloves but unlocking

    printf("Apprentice %d has removed gloves\n", apprentice_id);
}

void pick_ingredient(int apprentice_id, int* meal_index, int* ing_index) {
   if (exe_done == 0) {
    put_gloves(apprentice_id);
    pthread_mutex_lock(&ingredient_mutex); // For Preventing Race Condition (Locking)
    // Implement a control mechanism here using mutexes if needed
    *meal_index = pick_meal_counter;
    *ing_index = meal_ing_number;
 
    if (*meal_index < 4) {
    printf("Apprentice %d has taken ingredient %s\n", apprentice_id, Menu[*meal_index].ingredients[*ing_index].name);
    }

    // Possible Race condition here, take your precaution
    if (meal_ing_number == 2) {
    // maximum 3 ingredients
      meal_ing_number = 0;
      pick_meal_counter = pick_meal_counter + 1;

    } else { //push for more
      meal_ing_number = meal_ing_number + 1;
    }

    pthread_mutex_unlock(&ingredient_mutex); // For Preventing Race Condition (Unlocking)
    remove_gloves(apprentice_id);
   }
} 

void prepare_ingredient(int apprentice_id, int meal_index, int ing_index) {
    if (meal_index < 4)
      printf("Apprentice %d is preparing: %s \n", apprentice_id, Menu[meal_index].ingredients[ing_index].name);
    sleep(Menu[meal_index].ingredients[ing_index].time_to_process);
    if (meal_index < 4)
      printf("Apprentice %d is done preparing %s \n", apprentice_id, Menu[meal_index].ingredients[ing_index].name);
}

void put_ingredient(int id, int meal_index, int ing_index) {
    while(1) {
      // checking pot counts if it is less than 3 and other control mechanisms such as if exe is done and if meal index is equls to the meal counter (control mechanism)
      if (pot_counter < 3 && exe_done == 0 && meal_index == meal_counter) {
        if (meal_index < 4)
          printf("Apprentince %d is trying to put %s into pot\n", id, Menu[meal_index].ingredients[ing_index].name);
        // Implement a control mechanism here using mutexes to prevent the second problem mentioned in HW file
        pthread_mutex_lock(&pot_mutex);
        if (meal_index < 4)
          printf("Apprentince %d has put %s into pot\n", id, Menu[meal_index].ingredients[ing_index].name);
        pot_counter += 1;
        pthread_mutex_unlock(&pot_mutex);
        break;
        // Do not forget to break the loop !
      }
    }
}

void help_chef(int apprentice_id) {
    int meal_index, meal_ingredient_index;
    pick_ingredient(apprentice_id, &meal_index, &meal_ingredient_index);
    prepare_ingredient(apprentice_id, meal_index, meal_ingredient_index);
    put_ingredient(apprentice_id, meal_index, meal_ingredient_index);
}

void *apprentice(int *apprentice_id) {
    printf("Im apprentice %d\n", *apprentice_id);
    while(1) {
      if(meal_counter < 4){
        help_chef(*apprentice_id);
      }else{ 
        break; // meals are done
      }
    }
    pthread_exit(NULL);
}

void *chef() {
    while(1) {
        /** Chef works but you need to implement a control mechanism for him 
         *  to prevent the second problem mentioned in HW file.
         *  As for now, he just cooks without checking the pot.
         */
        if(pot_counter == 3){
        printf("Chef is preparing meal %s\n", Menu[meal_counter].name);
        sleep(Menu[meal_counter].time_to_prepare);
        printf("Chef prepared the meal %s\n",  Menu[meal_counter].name);
        if (meal_counter == 4){
        exe_done = 1;
        } 
	// To prevent race condition scenarios we should implement a mutex here agan LOCK spesific ingredient mutex

        pthread_mutex_lock(&ingredient_mutex);

        //meal_ing_number = 0;// should i use this or not im not sure
        meal_counter = meal_counter + 1;
        pot_counter = 0;

        pthread_mutex_unlock(&ingredient_mutex);
        // To prevent race condition scenarios we should implement a mutex here agan UNLOCK spesific ingredient mutex

        sleep(3); // Let the chef rest after preparing the meal
        if(meal_counter == MEALS_TO_PREPARE) // If all meals are prepared chef can go home
            break;
      }
    }
    pthread_exit(NULL);
}



int main() {
    pthread_t apprentice_threads[APPRENTICE_COUNT];
    pthread_t chef_thread;
    void *apprentice();

    int apprentice_ids[APPRENTICE_COUNT] = {0 ,1 ,2};
    
    // Initialize Glove mutexes here  done
    for (size_t i=0;i<GLOVE_COUNT;i++)
        pthread_mutex_init(&glove_mutex[i],NULL);

    // Initialize threads here done

    // Should i use this i am not too sure we normally used these lines in our lectures
    //void *chef();
    //void *apprentice();

    for (size_t i = 0; i < APPRENTICE_COUNT; i++)
        pthread_create(&apprentice_threads[i],NULL,apprentice,&apprentice_ids[i]);
    
    pthread_create(&chef_thread,NULL,chef,NULL);

    // Tell the main thread to wait for other threads to complete
    for (size_t i = 0; i < APPRENTICE_COUNT; i++)
        pthread_join(apprentice_threads[i],NULL);

    pthread_join(chef_thread, NULL);

    return 0;
}
