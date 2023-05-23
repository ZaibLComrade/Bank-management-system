nt accType = menu();
    Prompts login based on account type selected in menu 

    switch(accType) {
        case 1: 
            customerPrompt();
            break;
        case 2: 
            managerPrompt();
            break;
        case 3: 
            adminPrompt();
            break;
    }