struct i2c_client {                                                                                                                                                       
    unsigned short flags;       /* div., see below      */                                                                                                                
    unsigned short addr;        /* chip address - NOTE: 7bit    */                                                                                                        
                    /* addresses are stored in the  */                                                                                                                    
                    /* _LOWER_ 7 bits       */                                                                                                                            
    char name[I2C_NAME_SIZE];                                                                                                                                             
    struct i2c_adapter *adapter;    /* the adapter we sit on    */                                                                                                        
    struct i2c_driver *driver;  /* and our access routines  */                                                                                                            
    struct device dev;      /* the device structure     */                                                                                                                
    int irq;            /* irq issued by device     */                                                                                                                    
    struct list_head detected;                                                                                                                                            
};  

struct i2c_board_info {                                                                                                                                                   
     char        type[I2C_NAME_SIZE];                                                                                                                                      
     unsigned short  flags;                                                                                                                                                
     unsigned short  addr;                                                                                                                                                 
     void        *platform_data;                                                                                                                                           
     struct dev_archdata *archdata;                                                                                                                                        
     struct device_node *of_node;                                                                                                                                          
     struct acpi_dev_node acpi_node;                                                                                                                                       
     int     irq;                                                                                                                                                          
 };

struct i2c_adapter {                                                                                                                                                      
    struct module *owner;                                                                                                                                                 
    unsigned int class;       /* classes to allow probing for */                                                                                                          
    const struct i2c_algorithm *algo; /* the algorithm to access the bus */                                                                                               
    void *algo_data;                                                                                                                                                      
                                                                                                                                                                          
    /* data fields that are valid for all devices   */                                                                                                                    
    struct rt_mutex bus_lock;                                                                                                                                             
                                                                                                                                                                          
    int timeout;            /* in jiffies */                                                                                                                              
    int retries;                                                                                                                                                          
    struct device dev;      /* the adapter device */                                                                                                                      
                                                                                                                                                                          
    int nr;                                                                                                                                                               
    char name[48];                                                                                                                                                        
    struct completion dev_released;                                                                                                                                       
                                                                                                                                                                          
    struct mutex userspace_clients_lock;                                                                                                                                  
    struct list_head userspace_clients;                                                                                                                                   
                                                                                                                                                                          
    struct i2c_bus_recovery_info *bus_recovery_info;                                                                                                                      
};
struct i2c_driver {                                                                                                                                                       
    unsigned int class;                                                                                                                                                   
                                                                                                                                                                          
    /* Notifies the driver that a new bus has appeared. You should avoid                                                                                                  
     * using this, it will be removed in a near future.                                                                                                                   
     */                                                                                                                                                                   
    int (*attach_adapter)(struct i2c_adapter *) __deprecated;                                                                                                             
                                                                                                                                                                          
    /* Standard driver model interfaces */                                                                                                                                
    int (*probe)(struct i2c_client *, const struct i2c_device_id *);                                                                                                      
    int (*remove)(struct i2c_client *);                                                                                                                                   
                                                                                                                                                                          
    /* driver model interfaces that don't relate to enumeration  */                                                                                                       
    void (*shutdown)(struct i2c_client *);                                                                                                                                
    int (*suspend)(struct i2c_client *, pm_message_t mesg);                                                                                                               
    int (*resume)(struct i2c_client *);                                                                                                                                   
                                                                                                                                                                          
    /* Alert callback, for example for the SMBus alert protocol.                                                                                                          
     * The format and meaning of the data value depends on the protocol.                                                                                                  
     * For the SMBus alert protocol, there is a single bit of data passed                                                                                                 
     * as the alert response's low bit ("event flag").                                                                                                                    
     */                                                                                                                                                                   
    void (*alert)(struct i2c_client *, unsigned int data);                                                                                                                
                                                                                                                                                                          
    /* a ioctl like command that can be used to perform specific functions                                                                                                
     * with the device.                                                                                                                                                   
     */                                                                                                                                                                   
    int (*command)(struct i2c_client *client, unsigned int cmd, void *arg);                                                                                               
                                                                                                                                                                          
    struct device_driver driver;                                                                                                                                          
    const struct i2c_device_id *id_table;                                                                                                                                 
                                                                                                                                                                          
    /* Device detection callback for automatic device creation */                                                                                                         
    int (*detect)(struct i2c_client *, struct i2c_board_info *);                                                                                                          
    const unsigned short *address_list;                                                                                                                                   
    struct list_head clients;                                                                                                                                             
}; 
#define I2C_NAME_SIZE 20
struct i2c_device_id {                                           
    char name[I2C_NAME_SIZE];                                    
    kernel_ulong_t driver_data; /* Data private to the driver */ 
}; 

struct i2c_board_info {                
     char        type[I2C_NAME_SIZE];  
     unsigned short  flags;            
     unsigned short  addr;             
     void        *platform_data;       
     struct dev_archdata *archdata;    
     struct device_node *of_node;      
     struct acpi_dev_node acpi_node;   
     int     irq;                      
 };                                    
#define I2C_BOARD_INFO(dev_type, dev_addr) .type = dev_type, .addr = (dev_addr)   
                                                              

