#include "power.h"

passive_power::~passive_power()
{
    for(i = 0;i < nb;++i)
    {
        if(type[i] == POW_P_DICE)
            delete (passive_dice*)power[i];
        else if(type[i] == POW_P_GAUGE)
            delete (passive_gauge*)power[i];
    }
    free(power);
    free(type);
}

active_power::~active_power()
{
    for(i = 0;i < nb;++i)
    {
        if(type[i] == POW_A_GAUGE)
            delete (passive_dice*)power[i];
        else if(type[i] == POW_A_VARIABLE)
            delete (passive_gauge*)power[i];
        else if(type[i] == POW_A_INVENTORY)
            delete (passive_gauge*)power[i];
        else if(type[i] == POW_A_EFFECT)
            delete (passive_gauge*)power[i];
        else if(type[i] == POW_A_SAY)
            delete (passive_gauge*)power[i];
        else if(type[i] == POW_A_FEATURE)
            delete (passive_gauge*)power[i];
        else if(type[i] == POW_A_STAT)
            delete (passive_gauge*)power[i];
    }
    free(power);
    free(type);
    free(mute);
}

equiped_power::~equiped_power()
{
    for(i = 0;i < nb;++i)
    {
        if(type[i] == POW_E_DICE)
            delete (equiped_dice*)power[i];
        else if(type[i] == POW_E_GAUGE)
            delete (equiped_gauge*)power[i];
    }
    free(power);
    free(type);
}


bool passive_power::get_all(FILE *file,compilateur* output)
{
    tpmtell = ftell(file);
    nb = 0;
    while(get_passive_power(file)) nb++;
    if(nb == 0) return 1;
    fseek(file,tpmtell,SEEK_SET);
    type = new type_power[nb];
    power = (void**)malloc(sizeof(void*)*nb);
    sprintf(blanket,"%ld passive code détecté\n",nb);
    output->add(blanket);
    for(i = 0;i < nb;++i)
    {
        type[i] = get_passive_power(file);
        if(type[i] == POW_P_DICE)
        {
            output->add("compilation de INSIDE_CODE type DICE : ");
            power[i] = new class passive_dice;
            ((passive_dice*)power[i])->setup();
            take_spec_id(file,((passive_dice*)power[i])->id,&((passive_dice*)power[i])->minus);
            fscanf(file," %s",blanket);
            output->add(((passive_dice*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"MALUS"))
                ((passive_dice*)power[i])->action = ADDMIN_MALUS;
            else if(!strcmp(blanket,"BONUS"))
                ((passive_dice*)power[i])->action = ADDMIN_BONUS;
            else if(!strcmp(blanket,"UNUSED"))
                ((passive_dice*)power[i])->action = ADDMIN_UNUSED;
            else if(!strcmp(blanket,"USED"))
                ((passive_dice*)power[i])->action = ADDMIN_USED;
            else if(!strcmp(blanket,"CLEAR"))
                ((passive_dice*)power[i])->action = ADDMIN_CLEAR;

            if(((passive_dice*)power[i])->action == ADDMIN_MALUS || ((passive_dice*)power[i])->action == ADDMIN_BONUS)
                ((passive_dice*)power[i])->x.get(file);

        }
        else if(type[i] == POW_P_GAUGE)
        {
            output->add("compilation de INSIDE_CODE type GAUGE : ");
            power[i] = new class passive_gauge;
            ((passive_gauge*)power[i])->setup();
            take_spec_id(file,((passive_gauge*)power[i])->id,&((passive_gauge*)power[i])->minus);
            fscanf(file,"MAX %s",blanket);
            output->add(((passive_gauge*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"ADD"))
                ((passive_gauge*)power[i])->action = ADDMIN_ADD;
            else if(!strcmp(blanket,"REMOVE"))
                ((passive_gauge*)power[i])->action = ADDMIN_REMOVE;
            else if(!strcmp(blanket,"SET"))
                ((passive_gauge*)power[i])->action = ADDMIN_SET;

            ((passive_gauge*)power[i])->x.get(file);
        }
        else
            power[i] = NULL;
    }
    return 0;
}

bool active_power::get_all(FILE *file,compilateur* output)
{
    tpmtell = ftell(file);
    nb = 0;
    while(get_active_power(file,NULL)) nb++;
    if(nb == 0) return 1;
    fseek(file,tpmtell,SEEK_SET);
    type = new type_power[nb];
    mute = new bool[nb];
    power = (void**)malloc(sizeof(void*)*nb);
    sprintf(blanket,"%ld active code détecté\n",nb);
    output->add(blanket);
    for(i = 0;i < nb;++i)
    {
        type[i] = get_active_power(file,&mute[i]);
        if(type[i] == POW_A_GAUGE)
        {
            output->add("compilation de INSIDE_CODE type GAUGE : ");
            power[i] = new class active_gauge;
            ((active_gauge*)power[i])->setup();
            take_spec_id(file,((active_gauge*)power[i])->id,&((active_gauge*)power[i])->minus);
            fscanf(file," ACTUAL %s",blanket);
            output->add(((active_gauge*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"ADD"))
                ((active_gauge*)power[i])->action = ADDMIN_ADD;
            else if(!strcmp(blanket,"REMOVE"))
                ((active_gauge*)power[i])->action = ADDMIN_REMOVE;
            else if(!strcmp(blanket,"SET"))
                ((active_gauge*)power[i])->action = ADDMIN_SET;

            ((active_gauge*)power[i])->x.get(file);
        }
        else if(type[i] == POW_A_VARIABLE)
        {
            output->add("compilation de INSIDE_CODE type VARIABLE : ");
            power[i] = new class active_variable;
            ((active_variable*)power[i])->setup();
            take_spec_id(file,((active_variable*)power[i])->id,&((active_variable*)power[i])->minus);
            fscanf(file," %s",blanket);
            output->add(((active_variable*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"ADD"))
                ((active_variable*)power[i])->action = ADDMIN_ADD;
            else if(!strcmp(blanket,"REMOVE"))
                ((active_variable*)power[i])->action = ADDMIN_REMOVE;
            else if(!strcmp(blanket,"SET"))
                ((active_variable*)power[i])->action = ADDMIN_SET;

            ((active_variable*)power[i])->x.get(file);
        }
        else if(type[i] == POW_A_INVENTORY)
        {
            output->add("compilation de INSIDE_CODE type INVENTORY : ");
            power[i] = new class active_inventory;
            ((active_inventory*)power[i])->setup();
            take_spec_id(file,((active_inventory*)power[i])->id,NULL);
            fscanf(file," %s",blanket);
            output->add(((active_inventory*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"ADD"))
                ((active_inventory*)power[i])->action = ADDMIN_ADD;
            else if(!strcmp(blanket,"REMOVE"))
                ((active_inventory*)power[i])->action = ADDMIN_REMOVE;
            else if(!strcmp(blanket,"SET"))
                ((active_inventory*)power[i])->action = ADDMIN_SET;

            ((active_inventory*)power[i])->x.get(file);

            if(fgetc(file) == ' ')
            {
                fscanf(file,"%s",blanket);
                if(!strcmp(blanket,"PERSO"))
                {
                    ((active_inventory*)power[i])->perso = 1;
                    ((active_inventory*)power[i])->info = (void*)ftell(file);
                }
            }
        }
        else if(type[i] == POW_A_EFFECT)
        {
            output->add("compilation de INSIDE_CODE type EFFECT : ");
            power[i] = new class active_effect;
            ((active_effect*)power[i])->setup();
            take_spec_id(file,((active_effect*)power[i])->id,NULL);
            fscanf(file," %s",blanket);
            output->add(((active_effect*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"ADD"))
                ((active_effect*)power[i])->action = ADDMIN_ADD;
            else if(!strcmp(blanket,"REMOVE"))
                ((active_effect*)power[i])->action = ADDMIN_REMOVE;
            else if(!strcmp(blanket,"SET"))
                ((active_effect*)power[i])->action = ADDMIN_SET;

            ((active_effect*)power[i])->x.get(file);

            if(fgetc(file) == ' ')
            {
                fscanf(file,"%s",blanket);
                if(!strcmp(blanket,"PERSO"))
                {
                    ((active_effect*)power[i])->perso = 1;
                    ((active_effect*)power[i])->info = (void*)ftell(file);
                }
            }
        }
        else if(type[i] == POW_A_SAY)
        {
            output->add("compilation de INSIDE_CODE type SAY\n");
            fgetc(file);
            tpmtell = ftell(file);
            tpm = 1;
            while(fgetc(file) != '\n') tpm++;
            ((active_say*)power[i])->something = new char[tpm];
            fseek(file,tpmtell,SEEK_SET);
            tpm--;
            for(i = 0;i < tpm;++i)
                ((active_say*)power[i])->something[i] = fgetc(file);
            ((active_say*)power[tpm])->something[tpm] = '\0';
        }
        else if(type[i] == POW_A_FEATURE)
        {
            output->add("compilation de INSIDE_CODE type FEATURE : ");
            power[i] = new class active_feature;
            ((active_feature*)power[i])->setup();
            take_spec_id(file,((active_feature*)power[i])->id,NULL);
            fscanf(file," %s",blanket);
            output->add(((active_feature*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"ADD"))
                ((active_feature*)power[i])->action = ADDMIN_ADD;
            else if(!strcmp(blanket,"REMOVE"))
                ((active_feature*)power[i])->action = ADDMIN_REMOVE;
            else if(!strcmp(blanket,"SET"))
                ((active_feature*)power[i])->action = ADDMIN_SET;

            ((active_feature*)power[i])->x.get(file);

            if(fgetc(file) == ' ')
            {
                fscanf(file,"%s",blanket);
                if(!strcmp(blanket,"PERSO"))
                {
                    ((active_feature*)power[i])->perso = 1;
                    ((active_feature*)power[i])->info = (void*)ftell(file);
                }
            }
        }
        else if(type[i] == POW_A_STAT)
        {
            output->add("compilation de INSIDE_CODE type STAT : ");
            power[i] = new class active_stat;
            ((active_stat*)power[i])->setup();
            take_spec_id(file,((active_stat*)power[i])->id,NULL);
            fscanf(file," %s",blanket);
            output->add(((active_stat*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"ADD"))
                ((active_stat*)power[i])->action = ADDMIN_ADD;
            else if(!strcmp(blanket,"REMOVE"))
                ((active_stat*)power[i])->action = ADDMIN_REMOVE;
            else if(!strcmp(blanket,"SET"))
                ((active_stat*)power[i])->action = ADDMIN_SET;

            ((active_gauge*)power[i])->x.get(file);
        }
        else
            power[i] = NULL;
    }
    return 0;
}

bool equiped_power::get_all(FILE *file,compilateur* output)
{
    tpmtell = ftell(file);
    nb = 0;
    while(get_equiped_power(file)) nb++;
    if(nb == 0) return 1;
    fseek(file,tpmtell,SEEK_SET);
    type = new type_power[nb];
    power = (void**)malloc(sizeof(void*)*nb);
    sprintf(blanket,"%ld equiped code détecté\n",nb);
    output->add(blanket);
    for(i = 0;i < nb;++i)
    {
        type[i] = get_passive_power(file);
        if(type[i] == POW_E_DICE)
        {
            output->add("compilation de INSIDE_CODE type DICE : ");
            power[i] = new class passive_dice;
            ((equiped_dice*)power[i])->setup();
            take_spec_id(file,((equiped_dice*)power[i])->id,&((equiped_dice*)power[i])->minus);
            fscanf(file," %s",blanket);
            output->add(((equiped_dice*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"MALUS"))
                ((equiped_dice*)power[i])->action = ADDMIN_MALUS;
            else if(!strcmp(blanket,"BONUS"))
                ((equiped_dice*)power[i])->action = ADDMIN_BONUS;
            else if(!strcmp(blanket,"UNUSED"))
                ((equiped_dice*)power[i])->action = ADDMIN_UNUSED;
            else if(!strcmp(blanket,"USED"))
                ((equiped_dice*)power[i])->action = ADDMIN_USED;
            else if(!strcmp(blanket,"CLEAR"))
                ((equiped_dice*)power[i])->action = ADDMIN_CLEAR;

            if(((equiped_dice*)power[i])->action == ADDMIN_MALUS || ((equiped_dice*)power[i])->action == ADDMIN_BONUS)
                ((equiped_dice*)power[i])->x.get(file);

        }
        else if(type[i] == POW_E_GAUGE)
        {
            output->add("compilation de INSIDE_CODE type GAUGE : ");
            power[i] = new class passive_gauge;
            ((equiped_gauge*)power[i])->setup();
            take_spec_id(file,((equiped_gauge*)power[i])->id,&((equiped_gauge*)power[i])->minus);
            fscanf(file,"MAX %s",blanket);
            output->add(((equiped_gauge*)power[i])->id);output->add(" effet : ");output->add(blanket);output->add("\n");
            if(!strcmp(blanket,"ADD"))
                ((equiped_gauge*)power[i])->action = ADDMIN_ADD;
            else if(!strcmp(blanket,"REMOVE"))
                ((equiped_gauge*)power[i])->action = ADDMIN_REMOVE;
            else if(!strcmp(blanket,"SET"))
                ((equiped_gauge*)power[i])->action = ADDMIN_SET;

            ((equiped_gauge*)power[i])->x.get(file);
        }
        else
            power[i] = NULL;
    }
    return 0;
}
