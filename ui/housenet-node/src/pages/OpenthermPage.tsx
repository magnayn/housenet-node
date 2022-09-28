import { Button, Stack, Table, TableBody, TableCell, TableHead, TableRow, TextField } from "@mui/material";
import { useEffect, useState } from "react";
import Configuration  from '../Config';

interface PageProps {
  id: string,
  config:any
}

export default function OpenthermPage(props:PageProps) {
    
    const [setPoint, setSetPoint] = useState<number>(0);
  
    function refresh()  {

      fetch(`${Configuration.baseUrl}/housenet/elements/opentherm/${props.id}`)           
      .then(res => res.json())
      .then(data => {
        setSetPoint(data.setpoint);
     
      });
    }

    useEffect( () => {
      refresh();
     
    }, []);

    function applySetpoint(value:number) {
      fetch(`${Configuration.baseUrl}/housenet/elements/opentherm/${props.id}`, {
        method: 'POST', body: "" + value
      }).then( r => refresh());
    }

    return (
     <div>
       <h1>OpenTherm</h1>
       <h2>{props.id}</h2>
       <p>Connection to OpenTherm device (e.g Boiler)</p>
       <Table size="small">
        <TableHead>
          
        </TableHead>
        <TableBody>         
            <TableRow >
              <TableCell>Pin In</TableCell>
              <TableCell>{props?.config.pinIn}</TableCell>
            </TableRow>          
            <TableRow >
              <TableCell>Pin Out</TableCell>
              <TableCell>{props?.config.pinOut}</TableCell>
            </TableRow>          
         
        </TableBody>
      </Table>
      <h2> Settings </h2>
        <div>
        <Stack spacing={2} direction="row">
       
          <TextField inputProps={{ inputMode: 'numeric', pattern: '[0-9]*' }} value={setPoint} onChange={(v)=>setSetPoint( parseInt(v.target.value) )} />
          <Button variant="contained" onClick={()=>applySetpoint(setPoint)}>Apply</Button>
          </Stack>
        </div>


      </div>
        
    );
}
