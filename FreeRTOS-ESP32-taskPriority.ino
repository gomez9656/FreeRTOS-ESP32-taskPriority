//use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

const char msg[] = "Hello hello";

//task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

 //Task: print to serial monitor with lower priority
void startTask1(void *parameter){

  int msg_len = strlen(msg);

  while(1){
    
    Serial.print("");
    
    for(int i = 0; i < msg_len; i++){
      Serial.println(msg[i]);
    }
    
    Serial.print("");
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//task: print to serial terminal with higher priority
void startTask2(void *parameter){

  while(1){

    Serial.print("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  //low baud rate to see the task happening 
  Serial.begin(300);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("FreeRTOS demo");

  Serial.print("Setup and loop task running on core: ");
  Serial.print(xPortGetCoreID());
  Serial.print("with priority");
  Serial.println(uxTaskPriorityGet(NULL));

  //tasks to run forever
  xTaskCreatePinnedToCore(startTask1,
                          "Task 1",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);
                          
  xTaskCreatePinnedToCore(startTask2,
                          "Task 2",
                          1024,
                          NULL,
                          2,
                          &task_2,
                          app_cpu);

}

void loop() {

  //suspends the higher priority task for some intervals
  for(int i = 0; i < 3; i++){

    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  //delete the lower priotity task
  if(task_1 != NULL){

    vTaskDelete(task_1);
    task_1 = NULL;
  }
}
