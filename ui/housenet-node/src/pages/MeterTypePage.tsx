
import { Button, Stack, Table, TableBody, TableCell, TableHead, TableRow, TextField } from "@mui/material";
import { useEffect, useRef, useState } from "react";
import { useIntervalWhen } from "rooks";
import Configuration from '../Config';



interface MeterProp {
  value: number
  significantFigures: number
}

function digit(n:number, dig:number):number {

  
    return Math.floor((n % (10**dig)) / (10**(dig-1))) ;

}

function GasMeter(props:MeterProp) {
  return <Stack className="gasMeter" spacing={2} direction="row">
    <div>{digit(props.value, 6)}</div>
    <div>{digit(props.value, 5)}</div>
    <div>{digit(props.value, 4)}</div>
    <div>{digit(props.value, 3)}</div>
    <div className="leastSignificant">{digit(props.value, 2)}</div>
    <div className="leastSignificant">{digit(props.value, 1)}</div>
</Stack>
}

function ElectricMeter(props:MeterProp) {

  var sigFigs = [];

  sigFigs.push(<div>.</div>);
  let x = 3;

  for( let i=0; i< props.significantFigures; i++ ) {
    sigFigs.push(<div>{digit(props.value, x)}</div>);
    x = x - 1;
  }
  
  let majFigs = [];
  for( let i=0; i<5; i++ ) {
    majFigs.unshift(<div>{digit(props.value, 4+i)}</div>)
  }

  return <Stack className="electricMeter" spacing={2} direction="row">
    {majFigs}    
    { props.significantFigures > 0 && sigFigs }  
    
</Stack>
}

interface PageProps {
  id: string,
  config: any
}
export default function MeterTypePage(props: PageProps) {

  const [configuration, setConfiguration] = useState<any>({});
  const [initialValue, setInitialValue] = useState<number>(0);
  const [currentValue, setCurrentValue] = useState<number>(0);

  const [fetched, setFetched] = useState<boolean>(false);


  function refresh() {
    console.log(`Refresh ${props.id}`);

    fetch(`${Configuration.baseUrl}/housenet/elements/meter/${props.id}`)
      .then(res => res.json())
      .then(data => {
        setConfiguration(data);

        

        if (!fetched) {
          console.log("Set");
          setFetched(true);
          setInitialValue(data.meter);
        }
        setCurrentValue(data.meter);
      });
  }

  useEffect(() => {
    console.log("CDU");
    setFetched(false);
    refresh();
  },[props.id]);
  
  useIntervalWhen(
    () => {
      refresh();
    },
    750, // run callback every 1 second
    true, // start the timer when it's true
    true // no need to wait for the first interval
  );


  function setMeterValue(value: number) {

    fetch(`${Configuration.baseUrl}/housenet/elements/meter/${props.id}`, {
      method: 'POST', body: "=" + value
    }).then(r => refresh());

  }

  function adjustMeterValue(value: number) {
    fetch(`${Configuration.baseUrl}/housenet/elements/meter/${props.id}`, {
      method: 'POST', body: "" + value
    }).then(r => refresh());
  }

  return (
    <div>
      <h1>Meter</h1>
      <h2>{props.id}</h2>
      <p>Count the number of pulses and store as a metered value.</p>
      <Table size="small">
        <TableHead>

        </TableHead>
        <TableBody>
          <TableRow >
            <TableCell>Pin</TableCell>
            <TableCell>{props.config?.pin}</TableCell>
          </TableRow>
          <TableRow >
            <TableCell>Debounce</TableCell>
            <TableCell>{props.config?.debounce}</TableCell>
          </TableRow>

          <TableRow >
            <TableCell>Pulse Count</TableCell>
            <TableCell>{configuration.pulses}</TableCell>
          </TableRow>

          <TableRow >
            <TableCell>Meter Value</TableCell>
            <TableCell>{configuration.meter_initialized ? "" : "("} {configuration.meter} {configuration.meter_initialized ? "" : ")"} </TableCell>
          </TableRow>

        </TableBody>
        </Table>
        <h2> Settings </h2>
        <div>
          <Stack spacing={2} direction="column">
            <div><p>Current Value: 
              {props.config?.ui?.style == 'gas' ? 
                <GasMeter value={currentValue} significantFigures={props.config?.ui?.significantFigures ?? 0}></GasMeter> :
                <ElectricMeter value={currentValue} significantFigures={props.config?.ui?.significantFigures ?? 0}></ElectricMeter>
              }
              </p></div>
            
            <TextField inputProps={{ inputMode: 'numeric', pattern: '[0-9]*' }} value={initialValue} onChange={(v) => setInitialValue(parseInt(v.target.value))} />
            <Button variant="contained" onClick={() => setMeterValue(initialValue)}>Set Meter Value</Button>
          </Stack>
        </div>
        <br />
        <div>
          <Stack spacing={2} direction="row">
            <Button variant="contained" onClick={() => adjustMeterValue(100)}>+100</Button>
            <Button variant="contained" onClick={() => adjustMeterValue(10)}>+10</Button>
            <Button variant="contained" onClick={() => adjustMeterValue(1)}>+1</Button>
            <Button variant="contained" onClick={() => adjustMeterValue(-1)}>-1</Button>
            <Button variant="contained" onClick={() => adjustMeterValue(-10)}>-10</Button>
            <Button variant="contained" onClick={() => adjustMeterValue(-100)}>-100</Button>

          </Stack>
        </div>
     
    </div>

  );
}
