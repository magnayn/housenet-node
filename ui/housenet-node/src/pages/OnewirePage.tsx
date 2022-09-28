import { Table, TableBody, TableCell, TableHead, TableRow, TextField } from "@mui/material";
import { useEffect, useState } from "react";
import Configuration  from '../Config';

interface PageProps {
  id: string,
  config:any
}

export default function OnewirePage(props:PageProps) {
    
    const [configuration, setConfiguration] = useState<any>({});
  
    function refresh()  {

      // fetch(`${Configuration.baseUrl}/housenet/elements/state/${props.id}`)           
      // .then(res => res.json())
      // .then(data => {
      //   setConfiguration(data);
     
      // });
    }

    // useEffect( () => {
    //   refresh();
    //   const timer = setInterval(() => refresh(), 750);
    //   return () => clearTimeout(timer);
    // }, []);

    return (
     <div>
       <h1>Onewire</h1>
       <h2>{props.id}</h2>
       <p>1-wire bus (e.g: Temperature sensors)</p>
       <Table size="small">
        <TableHead>
          
        </TableHead>
        <TableBody>         
            <TableRow >
              <TableCell>Pin</TableCell>
              <TableCell>{props?.config.pin}</TableCell>
            </TableRow>          
         
        </TableBody>
      </Table>
       
      </div>
        
    );
}
