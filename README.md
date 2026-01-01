# cha-OS *(ChaOS Operating System)*
- Simple Operating System Simulator with Hardware Level Abstraction.
- To build the OS:
	`make`
- To run the OS:
	`./chaOS`

# Available Console Commands:
```
		help                    - show recognised commands
        echo TEXT               - print TEXT as a string
        ps                      - list processes
        clear                   - clear screen, also works as "cls"
        mkdir DIR               - make new directory DIR
        ls                      - list available directories and files
                                -> ls var:      list available variables
        cd PATH                 - change current directory to PATH
        make FILE               - create an empty file with name FILE
        edit PATH               - edit the file present at the end of PATH, press enter twice to save
        cat PATH                - show the contents of the file at the end of PATH
        mv PATHA PATHB          - move a given file from PATHA to PATHB
        cp PATHA PATHB          - copy file from PATHA to PATHB
        var VARIABLE            - initialize new variable with name VARIABLE
        label LABEL             - initialize new label with name LABEL
        calc VARA OP VARB       - perform arithmetic or logical operation OP on variables VARA and VARB
                                - Available OP: +, -, *, /, %, &, |
                                - Answer stored in ANS Variable
        set VAR FILE            - write the value of variable VAR into the file FILE
        load FILE VAR           - write the value of file FILE into the variable VAR
        jump LABEL              - unconditional jump to LABEL
                                -> jump LABEL VAR:      jump to the label LABEL if VAR is non-zero
        del PATH                - delete the file at the end of PATH
        deldir PATH             - delete the directory at the end of PATH
        ./FILE                  - executes console commands present in FILE in sequential order
        exit                    - exit interface
```
