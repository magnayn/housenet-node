import { hsl, hsv, rgb } from 'color-convert';
import React, { Fragment, useState } from 'react';
import { GeneratedIdentifierFlags } from 'typescript';


const color_red = "#ffc0c0";
const color_green = "#c0ffc0";

const text_green = "#235e00";
const text_red   = "#b1001c"




function PowerPanel(props:any) {

    var electric = props.view;

    var gen_amount = electric.solar.generate / 3200;
    
    // 120 hue is green, 0 is red.
    // v of 100 is white->color, 0 is black->color
    //var h = rgb.hsv(120,gen_amount,100);
    var h = hsv.rgb([120,gen_amount*100,100]);    
    var h2 = "#" + rgb.hex(h);

    // What % of energy is solar currently?
    var prop = electric.solar.consume / electric.location.use;

    // Hue is the colour of our energy right now
    var hue = 120 * prop;


    function power_colour(consumption:number, max:number) {
        return "#" + rgb.hex( hsv.rgb([hue,consumption/max*100,100]));
    }

    // solar: {
    //     generate: generate,
    //     export: _export,
    //     consume: solarIn
    // },
    // grid: {
    //     import: grid>0?grid:0,
    //     export: grid<0?-grid:0
    // },
    // location: {
    //     use: inUse,
    //     flat: { 
    //         use: inUseFlat
    //     },
    //     house: {
    //         use: inUseHoue,
    //         fuseboxes: [
    //             { use: fb1},
    //             { use: fb2},
    //             { use: fb3}
    //         ],
    //         smartplugs: [

    console.log(h2);

    return (
        <svg
        xmlns="http://www.w3.org/2000/svg"
        viewBox="109 232 1082 318"
        width={1082}
        height={318}
        {...props}
      >
        <g fill="none">
            {electric.grid.import > 0 && <Fragment>
          <path
            d="M110 538.39v-19.78h70.36V508L215 528.5 180.36 549v-10.61z"
            fill="#ffc0c0"
          />
          <path
            d="M110 538.39v-19.78h70.36V508L215 528.5 180.36 549v-10.61z"
            stroke="gray"
            strokeLinecap="round"
            strokeLinejoin="round"
          />
          <text transform="translate(115 519.276)" fill="#000">
            <tspan
              fontFamily="Helvetica Neue"
              fontSize={16}
              fontWeight={400}
              x={17.852}
              y={15}
            >
              {electric.grid.import + " W"}
            </tspan>
          </text>
          </Fragment>}
          
          { electric.grid.export > 0 && <Fragment>
            <path
            d="M215 493.39v-19.78h-70.36V463L110 483.5l34.64 20.5v-10.61z"
            fill="#c0ffc0"
          />
          <path
            d="M215 493.39v-19.78h-70.36V463L110 483.5l34.64 20.5v-10.61z"
            stroke="gray"
            strokeLinecap="round"
            strokeLinejoin="round"
          />
          <text transform="translate(115 474.276)" fill="#000">
            <tspan
              fontFamily="Helvetica Neue"
              fontSize={16}
              fontWeight={400}
              x={17.852}
              y={15}
            >
              {electric.grid.export + " W"}
            </tspan>
          </text> </Fragment>
          }
          
          { electric.solar.export > 0 && <Fragment>
          <path
            d="M302.39 322h-19.78v106.36H272L292.5 463l20.5-34.64h-10.61z"
            fill="#c0ffc0"
          />
          <path
            d="M302.39 322h-19.78v106.36H272L292.5 463l20.5-34.64h-10.61z"
            stroke="gray"
            strokeLinecap="round"
            strokeLinejoin="round"
          />
          <text transform="rotate(-90 370.638 87.362)" fill="#235e00">
            <tspan
              fontFamily="Helvetica Neue"
              fontSize={16}
              fontWeight={400}
              x={35.852}
              y={15}
            >
              {electric.solar.export + " W"}
            </tspan>
          </text>
          </Fragment>}

          { electric.grid.import > 0 && 
          <Fragment>
          <path
            d="M370 515.89v-19.78h70.36V485.5L475 506l-34.64 20.5v-10.61z"
            fill="#ffc0c0"
          />
          <path
            d="M370 515.89v-19.78h70.36V485.5L475 506l-34.64 20.5v-10.61z"
            stroke="gray"
            strokeLinecap="round"
            strokeLinejoin="round"
          />
          <text transform="translate(375 496.776)" fill="#000">
            <tspan
              fontFamily="Helvetica Neue"
              fontSize={16}
              fontWeight={400}
              x={17.852}
              y={15}
            >
              {electric.grid.import + " W"}
            </tspan>
          </text>
          </Fragment>}
          <path
            d="M370 288.89v-19.78h70.36V258.5L475 279l-34.64 20.5v-10.61z"
            fill="#c0ffc0"
          />
          <path
            d="M370 288.89v-19.78h70.36V258.5L475 279l-34.64 20.5v-10.61z"
            stroke="gray"
            strokeLinecap="round"
            strokeLinejoin="round"
          />
          <text transform="translate(375 269.776)" fill="#235e00">
            <tspan
              fontFamily="Helvetica Neue"
              fontSize={16}
              fontWeight={400}
              x={17.852}
              y={15}
            >
              {electric.solar.consume + " W"}
            </tspan>
          </text>


          
          <path fill={h2} d="M215 236h155v86H215z" />
          <path
            stroke="gray"
            strokeLinecap="round"
            strokeLinejoin="round"
            d="M215 236h155v86H215z"
          />
          <text transform="translate(220 241)" fill="#000">
            <tspan
              fontFamily="Helvetica Neue"
              fontSize={16}
              fontWeight={700}
              x={40.052}
              y={16}
            >
              {"Solar PV"}
            </tspan>
          </text>
          <text transform="translate(267.156 290.492)" fill="#000" >
            <tspan
              fontFamily="Helvetica Neue"
              fontSize={16}
              fontWeight={700}
              x={0}
              y={16}
            >
              { electric.solar.generate + " W"}
            </tspan>
          </text>
          { /* GRID */ }
          <g>
            <path fill={electric.grid.import > 0 ? color_red:color_green} d="M215 463h155v86H215z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M215 463h155v86H215z"
            />
            <text transform="translate(220 468)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={16}
                fontWeight={700}
                x={21.836}
                y={16}
              >
                {"Grid Connect"}
              </tspan>
            </text>
            
            <g>
              <text transform="translate(267.156 518.768)" fill={electric.grid.import > 0 ? text_red : text_green}>
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={16}
                  fontWeight={700}
                  x={0}
                  y={16}
                >
                  {(electric.grid.import + electric.grid.export) + " W"}
                </tspan>
              </text>
            </g> 
          </g>
          <g>
            <path
              d="M630 515.89v-19.78h26.047V485.5l34.641 20.5-34.64 20.5v-10.61z"
              fill="#ffc0c0"
            />
            <path
              d="M630 515.89v-19.78h26.047V485.5l34.641 20.5-34.64 20.5v-10.61z"
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
            />
          </g>
          <g>
            <path
              d="M630 354.89v-19.78h26.047V324.5l34.641 20.5-34.64 20.5v-10.61z"
              fill="#ffc0c0"
            />
            <path
              d="M630 354.89v-19.78h26.047V324.5l34.641 20.5-34.64 20.5v-10.61z"
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
            />
          </g>
          <g>
            <path fill={power_colour(electric.location.use, 3000)} d="M475 236h155v313H475z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M475 236h155v313H475z"
            />
            <text transform="translate(480 241)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={16}
                fontWeight={700}
                x={18.268}
                y={16}
              >
                {"Dunstan Road"}
              </tspan>
            </text>
            <g>
              <text transform="translate(527.156 518.768)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={16}
                  fontWeight={700}
                  x={0}
                  y={16}
                >
                  {electric.location.use + " W"}
                </tspan>
              </text>
            </g>
          </g>
          <g>
            <path fill={power_colour(electric.location.flat.use, 3000)} d="M690.688 463h155v86h-155z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M690.688 463h155v86h-155z"
            />
            <text transform="translate(695.688 468)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={16}
                fontWeight={700}
                x={58.284}
                y={16}
              >
                {"Flat"}
              </tspan>
            </text>
            <g>
              <text transform="translate(742.844 518.768)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={16}
                  fontWeight={700}
                  x={0}
                  y={16}
                >
                  {electric.location.flat.use + " W"}
                </tspan>
              </text>
            </g>
          </g>
          <g>
            <path fill={power_colour(electric.location.house.use, 3000)} d="M690.688 236h155v218h-155z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M690.688 236h155v218h-155z"
            />
            <text transform="translate(695.688 241)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={16}
                fontWeight={700}
                x={48.052}
                y={16}
              >
                {"House"}
              </tspan>
            </text>
            <g>
              <text transform="translate(742.844 420)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={16}
                  fontWeight={700}
                  x={0}
                  y={16}
                >
                  {electric.location.house.use + " W"}
                </tspan>
              </text>
            </g>
          </g>
          <g>
            <path fill={power_colour(electric.location.house.fuseboxes[2].use, 3000)}  d="M862 386h155v68H862z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M862 386h155v68H862z"
            />
            <text transform="translate(867 391)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={16}
                fontWeight={700}
                x={33.38}
                y={16}
              >
                {"Fusebox 3"}
              </tspan>
            </text>
            <g>
              <text transform="translate(867 429.536)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={16}
                  fontWeight={700}
                  x={47.156}
                  y={16}
                >
                  {electric.location.house.fuseboxes[2].use + " W"}
                </tspan>
              </text>
            </g>
          </g>
          <g>
            <path fill={power_colour(electric.location.house.fuseboxes[1].use, 3000)}  d="M862 310.5h155v68H862z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M862 310.5h155v68H862z"
            />
            <text transform="translate(867 315.5)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={16}
                fontWeight={700}
                x={33.38}
                y={16}
              >
                {"Fusebox 2"}
              </tspan>
            </text>
            <g>
              <text transform="translate(867 354.536)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={16}
                  fontWeight={700}
                  x={47.156}
                  y={16}
                >
                  {electric.location.house.fuseboxes[1].use + " W"}
                </tspan>
              </text>
            </g>
          </g>
          <g>
            <path fill={power_colour(electric.location.house.fuseboxes[0].use, 3000)} d="M862 236h155v68H862z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M862 236h155v68H862z"
            />
            <text transform="translate(867 241)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={16}
                fontWeight={700}
                x={33.38}
                y={16}
              >
                {"Fusebox 1"}
              </tspan>
            </text>
            <g>
              <text transform="translate(867 279.536)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={16}
                  fontWeight={700}
                  x={47.156}
                  y={16}
                >
                  {electric.location.house.fuseboxes[0].use + " W"}
                </tspan>
              </text>
            </g>
          </g>
          <g>
            <path
              d="M845.688 273.367V262.75L862 270l-16.312 7.25v-3.883z"
              fill="#ffc0c0"
            />
            <path
              d="M845.688 273.367v-6.734h0v-3.883L862 270l-16.312 7.25v-3.883z"
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
            />
          </g>
          <g>
            <path
              d="M845.688 348.367V337.75L862 345l-16.312 7.25v-3.883z"
              fill="#ffc0c0"
            />
            <path
              d="M845.688 348.367v-6.734h0v-3.883L862 345l-16.312 7.25v-3.883z"
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
            />
          </g>
          <g>
            <path
              d="M845.688 423.367V412.75L862 420l-16.312 7.25v-3.883z"
              fill="#ffc0c0"
            />
            <path
              d="M845.688 423.367v-6.734h0v-3.883L862 420l-16.312 7.25v-3.883z"
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
            />
          </g>
          <g>
            <path fill={power_colour(electric.location.house.smartplugs[0].use, 1000)}  d="M1026 236h164v20h-164z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M1026 236h164v20h-164z"
            />
            <text transform="translate(1031 237.297)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={14}
                fontWeight={700}
                x={0}
                y={14}
              >
                {"SP1 : Server"}
              </tspan>
            </text>
            <g>
              <text transform="translate(1113 237.594)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={14}
                  fontWeight={700}
                  x={27.648}
                  y={14}
                >
                  {electric.location.house.smartplugs[0].use + " W"}
                </tspan>
              </text>
            </g>
          </g>
          <g>
            <path fill={power_colour(electric.location.house.smartplugs[1].use, 1000)} d="M1026 411h164v20h-164z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M1026 411h164v20h-164z"
            />
            <text transform="translate(1031 412.297)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={14}
                fontWeight={700}
                x={0}
                y={14}
              >
                {"SP2 : HW Tap"}
              </tspan>
            </text>
            <g>
              <text transform="translate(1113 412)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={14}
                  fontWeight={700}
                  x={27.648}
                  y={14}
                >
                  {electric.location.house.smartplugs[1].use + " W"}
                </tspan>
              </text>
            </g>
          </g>
          <g>
            <path fill={power_colour(electric.location.house.smartplugs[2].use, 1000)} d="M1026 262.203h164v20h-164z" />
            <path
              stroke="gray"
              strokeLinecap="round"
              strokeLinejoin="round"
              d="M1026 262.203h164v20h-164z"
            />
            <text transform="translate(1031 263.5)" fill="#000">
              <tspan
                fontFamily="Helvetica Neue"
                fontSize={14}
                fontWeight={700}
                x={0}
                y={14}
              >
                {"SP3 : ?"}
              </tspan>
            </text>
            <g>
              <text transform="translate(1113 263.5)" fill="#b1001c">
                <tspan
                  fontFamily="Helvetica Neue"
                  fontSize={14}
                  fontWeight={700}
                  x={27.648}
                  y={14}
                >
                  {electric.location.house.smartplugs[2].use + " W"}
                </tspan>
              </text>
            </g>
          </g>
        </g>
      </svg>
      );
}

export default PowerPanel;