import { Button, Stack, Table, TableBody, TableCell, TableHead, TableRow, TextField } from "@mui/material";
import { useEffect, useState } from "react";
import Configuration  from '../Config';

interface PageProps {
  id: string,
  config:any
}

export default function GPIOTriggerPage(props:PageProps) {
    
    const [setPoint, setSetPoint] = useState<number>(0);
  
    function refresh()  {

      fetch(`${Configuration.baseUrl}/housenet/elements/gpiotrigger/${props.id}`)           
      .then(res => res.json())
      .then(data => {
        setSetPoint(data.setpoint);
     
      });
    }

    useEffect( () => {
      refresh();
     
    }, []);

    function trigger() {
      fetch(`${Configuration.baseUrl}/housenet/elements/gpiotrigger/${props.id}`, {
        method: 'POST',body: "trigger"
      });
    }

    return (
     <div>
       <h1>GPIOTriggerPage</h1>
       <h2>{props.id}</h2>
       <p>Connection to GPIO line that gets pulled low for a period to activate it</p>
       <Table size="small">
        <TableHead>
          
        </TableHead>
        <TableBody>         
            <TableRow >
              <TableCell>Pin</TableCell>
              <TableCell>{props?.config.pin}</TableCell>
            </TableRow>          
            <TableRow >
              <TableCell>Delay</TableCell>
              <TableCell>{props?.config.delay}</TableCell>
            </TableRow>   
         
        </TableBody>
      </Table>


      <Button variant="contained" onClick={()=>trigger()}>Trigger</Button>
      

      </div>
        
    );
}
