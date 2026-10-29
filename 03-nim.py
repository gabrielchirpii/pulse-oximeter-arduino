
from random import randint
from random import random
ans = " "

while True :
    biglie = randint(10,100)
    bigliePC = 0
    biglieUM = 0
    #print("DEBUG", biglie)
    startPC = False
    startUM = False #forse inutile....
    if randint(1,2) == 1 :
        startPC = True
        print("inizio io!!")
    else :
        startPC = False
        print("Inizia tu")
    if randint(1,2) == 1 :
        intel = True 
    else :
        intel = False 
    notintel = not intel 

    if notintel :
        print("sono stupido")
        print("le biglie con cui giocheremo sono:", biglie)
        if startPC :
            while True :
                prPC = randint(1, int(biglie/2))
                bigliePC = bigliePC + prPC
                biglie = biglie - prPC
                print("ho pescato", prPC, "biglie")
                print("le biglie rimaste sono:", biglie)
                if biglie == 1 :
                    print("ho vinto!!")
                    break
                while True :
                    prUM = int(input("Quante biglie prendi? "))
                    if prUM > int((biglie/2)) :
                        print("non puoi prenderne più della metà")
                    else :
                        break
                biglieUM = biglieUM + prUM
                biglie = biglie - prUM
                print("le biglie rimaste sono:", biglie)
                if biglie == 1 :
                    print("Cavolo ho perso!!")
                    break
        else :
            while True :
                while True :
                    prUM = int(input("Quante biglie prendi? "))
                    if prUM > int((biglie/2)) :
                        print("non puoi prenderne più della metà")
                    else :
                        break
                biglieUM = biglieUM + prUM
                biglie = biglie - prUM
                print("le biglie rimaste sono: ", biglie)
                if biglie == 1 :
                    print("Cavolo ho perso")
                    break
                prPC = randint(1, int(biglie/2))
                bigliePC = bigliePC + prPC
                biglie = biglie - prPC
                print("ho pescato", prPC, "biglie")
                print("le biglie rimaste sono:", biglie)
                if biglie == 1 :
                    print("Ho vinto")
                    break
    else :
        print("sono intelligente")
        print("le biglie con cui giocheremo sono:", biglie)
        if startPC :
            while True :
                if biglie == 3 or biglie == 7 or biglie == 15 or biglie == 31 or biglie == 63 :
                    prPC = randint(1, int((biglie/2)))
                else:
                    while True :
                        prPC = randint(1, int(biglie/2))
                        biglieTemp = biglie - prPC
                        if biglieTemp == 1 or biglieTemp == 3 or biglieTemp == 7 or biglieTemp == 15 or biglieTemp == 31 or biglieTemp == 63 :
                            break
                bigliePC = bigliePC + prPC
                biglie = biglie - prPC
                print("ho pescato", prPC, "biglie")
                print("le biglie rimaste sono:", biglie)
                if biglie == 1 :
                    print("ho vinto!!")
                    break
                while True :
                    prUM = int(input("Quante biglie prendi? "))
                    if prUM > int((biglie/2)) :
                        print("non puoi prenderne più della metà")
                    else :
                        break
                biglieUM = biglieUM + prUM
                biglie = biglie - prUM
                print("le biglie rimaste sono:", biglie)
                if biglie == 1 :
                    print("Cavolo ho perso!!")
                    break
        else :
            while True :
                while True :
                    prUM = int(input("Quante biglie prendi? "))
                    if prUM > int((biglie/2)) :
                        print("non puoi prenderne più della metà")
                    else :
                        break
                biglieUM = biglieUM + prUM
                biglie = biglie - prUM
                print("le biglie rimaste sono:", biglie)
                if biglie == 1 :
                    print("Cavolo ho perso!!")
                    break
                if biglie == 3 or biglie == 7 or biglie == 15 or biglie == 31 or biglie == 63 :
                    prPC = randint(1, int((biglie/2)))
                else:
                    while True :
                        prPC = randint(1, int(biglie/2))
                        biglieTemp = biglie - prPC
                        if biglieTemp == 1 or biglieTemp == 3 or biglieTemp == 7 or biglieTemp == 15 or biglieTemp == 31 or biglieTemp == 63 :
                            break
                bigliePC = bigliePC + prPC
                biglie = biglie - prPC
                print("ho pescato", prPC, "biglie")
                print("le biglie rimaste sono:", biglie)
                if biglie == 1 :
                    print("ho vinto!!")
                    break
                
                

            
        

            
    
    
        
        
    print("simulazione")
    while True :
        ans = str(input("Vuoi giocare ancora?, inserisci N(no) o S(si): "))
        if ans == "S" or ans == "N" :
            break
    if ans == "N" :
        break

        
        
        
        
        
    


    
     
     
