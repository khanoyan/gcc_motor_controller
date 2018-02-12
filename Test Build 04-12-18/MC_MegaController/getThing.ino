/*************************

  Get Thing

  Parses Commands from Web Client. Header value of packet should be 183 and Footer is 237

**************************/

bool getThing(){

  if (XBEE_ON == true){
    return false;
  }

  else if(XBEE_ON == false){

    byte cmndRoverArr[3];
    byte cmndArmArr[4];

    if (Serial3.available() >= 4){            // if there are 4 or more than 4 bytes in the buffer go in the loop
      delay(100);
      byte header[1];                         // create a byte array to check for header
      Serial3.readBytes(header, 1);           // read the byte into header

      if (header[0] == 183){                  // check for header to be 183

        if (mode == MODE_ROVER){              // if rover

          Serial3.readBytes(cmndRoverArr, 3); // read bytes into array
          if (cmndRoverArr[2] == 237){        // check that ending byte is 234
                                              // if the ending byte is 234 then save data into the struct
            CMDS_TO_MC.direction = cmndRoverArr[0];
            CMDS_TO_MC.duration = cmndRoverArr[1];

            }
        }
        else if (mode == MODE_ARM){

          Serial3.readBytes(cmndArmArr, 4);

          if (cmndArmArr[3] == 237){          // check for ending byte, if true then save bytes into struct

            CMDS_TO_MC.direction = cmndArmArr[0];
            CMDS_TO_MC.duration = cmndArmArr[1];
            CMDS_TO_MC.motorNum = cmndArmArr[2];

          }
        }
      }
    }
  }
  return true;
}
