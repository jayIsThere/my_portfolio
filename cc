import React, { useState, useRef, useEffect } from 'react';
import {
  RadarChart, PolarGrid, PolarAngleAxis, PolarRadiusAxis, Radar, ResponsiveContainer,
  BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, Legend, LineChart, Line,
  ScatterChart, Scatter
} from 'recharts';
import * as d3 from 'd3';

const LibraryComparisonDemo = () => {
  const [activeLibrary, setActiveLibrary] = useState('recharts');
  const [activeChart, setActiveChart] = useState('radar');

  // D3 차트를 위한 ref들
  const d3RadarRef = useRef();
  const d3BarRef = useRef();
  const d3GaugeRef = useRef();
  const d3GroupedBarRef = useRef();
  const d3LineRef = useRef();
  const d3DotPlotRef = useRef();
  const d3SparklineRef = useRef();

  // 샘플 데이터
  const securityData = [
    { subject: '네트워크 보안', score: 85, fullMark: 100 },
    { subject: '데이터 보안', score: 72, fullMark: 100 },
    { subject: '접근 제어', score: 90, fullMark: 100 },
    { subject: '물리적 보안', score: 65, fullMark: 100 },
    { subject: '인적 보안', score: 78, fullMark: 100 },
    { subject: '사고 대응', score: 55, fullMark: 100 }
  ];

  const overallScore = Math.round(securityData.reduce((sum, item) => sum + item.score, 0) / securityData.length);

  const salesData = [
    { category: 'A', product1: 150, product2: 120, product3: 80 },
    { category: 'B', product1: 180, product2: 140, product3: 95 },
    { category: 'C', product1: 200, product2: 160, product3: 110 },
    { category: 'D', product1: 220, product2: 180, product3: 125 },
    { category: 'E', product1: 250, product2: 200, product3: 140 },
  ];

  const lineChartData = [
    { x: 'Jan', y: 30 },
    { x: 'Feb', y: 45 },
    { x: 'Mar', y: 40 },
    { x: 'Apr', y: 55 },
    { x: 'May', y: 50 },
    { x: 'Jun', y: 65 },
  ];

  const dotPlotData = [
    { id: 'Item A', min: 20, max: 80, value: 55 },
    { id: 'Item B', min: 10, max: 70, value: 45 },
    { id: 'Item C', min: 30, max: 90, value: 60 },
    { id: 'Item D', min: 25, max: 75, value: 40 },
  ];

  const sparklineData = [10, 25, 18, 30, 22, 15, 28, 35, 20, 27];

  // D3 레이더 차트
  const createD3Radar = () => {
    if (!d3RadarRef.current) return;

    const svg = d3.select(d3RadarRef.current);
    svg.selectAll("*").remove();

    const width = 400;
    const height = 400;
    const margin = 50;
    const radius = Math.min(width, height) / 2 - margin;

    const g = svg
      .attr("width", width)
      .attr("height", height)
      .append("g")
      .attr("transform", `translate(${width / 2}, ${height / 2})`);

    const angleSlice = Math.PI * 2 / securityData.length;

    // 격자 그리기
    const levels = 5;
    for (let i = 0; i < levels; i++) {
      const levelRadius = radius * (i + 1) / levels;
      g.append("circle")
        .attr("r", levelRadius)
        .attr("fill", "none")
        .attr("stroke", "#CDCDCD")
        .attr("stroke-width", 1);
    }

    // 축 그리기
    securityData.forEach((d, i) => {
      const angle = angleSlice * i - Math.PI / 2;
      const x = Math.cos(angle) * radius;
      const y = Math.sin(angle) * radius;

      g.append("line")
        .attr("x1", 0)
        .attr("y1", 0)
        .attr("x2", x)
        .attr("y2", y)
        .attr("stroke", "#CDCDCD")
        .attr("stroke-width", 1);

      g.append("text")
        .attr("x", x * 1.15)
        .attr("y", y * 1.15)
        .attr("text-anchor", "middle")
        .attr("dominant-baseline", "middle")
        .style("font-size", "12px")
        .text(d.subject);
    });

    // 데이터 영역 그리기
    const radarLine = d3.line()
      .x((d, i) => {
        const angle = angleSlice * i - Math.PI / 2;
        return Math.cos(angle) * (radius * d.score / 100);
      })
      .y((d, i) => {
        const angle = angleSlice * i - Math.PI / 2;
        return Math.sin(angle) * (radius * d.score / 100);
      })
      .curve(d3.curveLinearClosed);

    g.append("path")
      .datum(securityData)
      .attr("d", radarLine)
      .attr("fill", "#3B82F6")
      .attr("fill-opacity", 0.3)
      .attr("stroke", "#3B82F6")
      .attr("stroke-width", 2);
  };

  // D3 바 차트
  const createD3Bar = () => {
    if (!d3BarRef.current) return;

    const svg = d3.select(d3BarRef.current);
    svg.selectAll("*").remove();

    const margin = { top: 20, right: 30, bottom: 80, left: 40 };
    const width = 600 - margin.left - margin.right;
    const height = 300 - margin.bottom - margin.top;

    const g = svg
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .append("g")
      .attr("transform", `translate(${margin.left}, ${margin.top})`);

    const x = d3.scaleBand()
      .domain(securityData.map(d => d.subject))
      .range([0, width])
      .padding(0.1);

    const y = d3.scaleLinear()
      .domain([0, 100])
      .range([height, 0]);

    g.selectAll(".bar")
      .data(securityData)
      .enter().append("rect")
      .attr("class", "bar")
      .attr("x", d => x(d.subject))
      .attr("width", x.bandwidth())
      .attr("y", d => y(d.score))
      .attr("height", d => height - y(d.score))
      .attr("fill", "#3B82F6");

    g.append("g")
      .attr("transform", `translate(0, ${height})`)
      .call(d3.axisBottom(x))
      .selectAll("text")
      .style("text-anchor", "end")
      .attr("dx", "-.8em")
      .attr("dy", ".15em")
      .attr("transform", "rotate(-45)");

    g.append("g")
      .call(d3.axisLeft(y));
  };

  // D3 게이지 차트
  const createD3Gauge = () => {
    if (!d3GaugeRef.current) return;

    const svg = d3.select(d3GaugeRef.current);
    svg.selectAll("*").remove();

    const width = 200;
    const height = 150;
    const radius = 80;

    const g = svg
      .attr("width", width)
      .attr("height", height)
      .append("g")
      .attr("transform", `translate(${width / 2}, ${height - 20})`);

    const arc = d3.arc()
      .innerRadius(radius - 20)
      .outerRadius(radius)
      .startAngle(-Math.PI / 2)
      .endAngle(Math.PI / 2);

    // 배경 아크
    g.append("path")
      .attr("d", arc)
      .attr("fill", "#E5E7EB");

    // 점수 아크
    const scoreArc = d3.arc()
      .innerRadius(radius - 20)
      .outerRadius(radius)
      .startAngle(-Math.PI / 2)
      .endAngle(-Math.PI / 2 + (Math.PI * overallScore / 100));

    g.append("path")
      .attr("d", scoreArc)
      .attr("fill", "#3B82F6");

    // 점수 텍스트
    g.append("text")
      .attr("text-anchor", "middle")
      .attr("y", -10)
      .style("font-size", "24px")
      .style("font-weight", "bold")
      .text(overallScore);

    g.append("text")
      .attr("text-anchor", "middle")
      .attr("y", 10)
      .style("font-size", "14px")
      .text("점수");
  };

  // D3 그룹형 바 차트 (간단 구현)
  const createD3GroupedBar = () => {
    if (!d3GroupedBarRef.current) return;
    const svg = d3.select(d3GroupedBarRef.current);
    svg.selectAll("*").remove();

    const margin = { top: 20, right: 30, bottom: 40, left: 40 };
    const width = 600 - margin.left - margin.right;
    const height = 300 - margin.top - margin.bottom;

    const g = svg.attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .append("g")
      .attr("transform", `translate(${margin.left}, ${margin.top})`);

    const x0 = d3.scaleBand()
      .rangeRound([0, width])
      .paddingInner(0.1);

    const x1 = d3.scaleBand()
      .padding(0.05);

    const y = d3.scaleLinear()
      .rangeRound([height, 0]);

    const z = d3.scaleOrdinal()
      .range(['#8884d8', '#82ca9d', '#ffc658']);

    const keys = ['product1', 'product2', 'product3'];

    x0.domain(salesData.map(d => d.category));
    x1.domain(keys).rangeRound([0, x0.bandwidth()]);
    y.domain([0, d3.max(salesData, d => d3.max(keys, key => d[key]))]).nice();

    g.append("g")
      .selectAll("g")
      .data(salesData)
      .enter().append("g")
      .attr("transform", d => `translate(${x0(d.category)},0)`)
      .selectAll("rect")
      .data(d => keys.map(key => ({ key: key, value: d[key] })))
      .enter().append("rect")
      .attr("x", d => x1(d.key))
      .attr("y", d => y(d.value))
      .attr("width", x1.bandwidth())
      .attr("height", d => height - y(d.value))
      .attr("fill", d => z(d.key));

    g.append("g")
      .attr("class", "axis")
      .attr("transform", `translate(0,${height})`)
      .call(d3.axisBottom(x0));

    g.append("g")
      .attr("class", "axis")
      .call(d3.axisLeft(y).ticks(null, "s"));
  };

  // D3 라인 차트
  const createD3Line = () => {
    if (!d3LineRef.current) return;
    const svg = d3.select(d3LineRef.current);
    svg.selectAll("*").remove();

    const margin = { top: 20, right: 30, bottom: 40, left: 40 };
    const width = 600 - margin.left - margin.right;
    const height = 300 - margin.top - margin.bottom;

    const g = svg.attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .append("g")
      .attr("transform", `translate(${margin.left}, ${margin.top})`);

    const x = d3.scalePoint()
      .domain(lineChartData.map(d => d.x))
      .range([0, width]);

    const y = d3.scaleLinear()
      .domain(d3.extent(lineChartData, d => d.y))
      .range([height, 0]);

    const line = d3.line()
      .x(d => x(d.x))
      .y(d => y(d.y))
      .curve(d3.curveMonotoneX);

    g.append("g")
      .attr("transform", `translate(0,${height})`)
      .call(d3.axisBottom(x));

    g.append("g")
      .call(d3.axisLeft(y));

    g.append("path")
      .datum(lineChartData)
      .attr("fill", "none")
      .attr("stroke", "#3B82F6")
      .attr("stroke-width", 2)
      .attr("d", line);

    g.selectAll(".dot")
      .data(lineChartData)
      .enter().append("circle")
      .attr("class", "dot")
      .attr("cx", d => x(d.x))
      .attr("cy", d => y(d.y))
      .attr("r", 4)
      .attr("fill", "#3B82F6");
  };

  // D3 닷 플롯
  const createD3DotPlot = () => {
    if (!d3DotPlotRef.current) return;
    const svg = d3.select(d3DotPlotRef.current);
    svg.selectAll("*").remove();

    const margin = { top: 20, right: 30, bottom: 40, left: 40 };
    const width = 600 - margin.left - margin.right;
    const height = 300 - margin.top - margin.bottom;

    const g = svg.attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .append("g")
      .attr("transform", `translate(${margin.left}, ${margin.top})`);

    const x = d3.scalePoint()
      .domain(dotPlotData.map(d => d.id))
      .range([0, width]);

    const y = d3.scaleLinear()
      .domain([0, 100])
      .range([height, 0]);

    g.append("g")
      .attr("transform", `translate(0,${height})`)
      .call(d3.axisBottom(x));

    g.append("g")
      .call(d3.axisLeft(y));

    g.selectAll(".dot")
      .data(dotPlotData)
      .enter().append("circle")
      .attr("class", "dot")
      .attr("cx", d => x(d.id))
      .attr("cy", d => y(d.value))
      .attr("r", 6)
      .attr("fill", "#3B82F6");
  };

  // D3 스파크라인
  const createD3Sparkline = () => {
    if (!d3SparklineRef.current) return;
    const svg = d3.select(d3SparklineRef.current);
    svg.selectAll("*").remove();

    const width = 200;
    const height = 50;
    const margin = 2;

    const x = d3.scaleLinear()
      .domain([0, sparklineData.length - 1])
      .range([margin, width - margin]);

    const y = d3.scaleLinear()
      .domain(d3.extent(sparklineData))
      .range([height - margin, margin]);

    const line = d3.line()
      .x((d, i) => x(i))
      .y(d => y(d))
      .curve(d3.curveMonotoneX);

    svg.attr("width", width)
      .attr("height", height)
      .append("path")
      .datum(sparklineData)
      .attr("fill", "none")
      .attr("stroke", "#3B82F6")
      .attr("stroke-width", 2)
      .attr("d", line);
  };

  useEffect(() => {
    if (activeLibrary === 'd3') {
      if (activeChart === 'radar') createD3Radar();
      else if (activeChart === 'bar') createD3Bar();
      else if (activeChart === 'gauge') createD3Gauge();
      else if (activeChart === 'groupedBar') createD3GroupedBar();
      else if (activeChart === 'line') createD3Line();
      else if (activeChart === 'dotPlot') createD3DotPlot();
      else if (activeChart === 'sparkline') createD3Sparkline();
    }
  }, [activeLibrary, activeChart]);

  return (
    <div className="p-6 max-w-6xl mx-auto bg-gray-50 min-h-screen">
      <h1 className="text-3xl font-bold text-center mb-8 text-gray-800">
        차트 라이브러리별 보안 점수 시각화 비교
      </h1>

      {/* 라이브러리 선택 */}
      <div className="flex justify-center mb-6">
        <div className="bg-white rounded-lg p-1 shadow-md">
          {[
            { id: 'recharts', name: 'Recharts' },
            { id: 'd3', name: 'D3.js' },
            { id: 'nivo-style', name: 'Nivo' },
            { id: 'victory-style', name: 'Victory' },
          ].map(lib => (
            <button
              key={lib.id}
              onClick={() => setActiveLibrary(lib.id)}
              className={`px-6 py-2 rounded-md transition-all ${
                activeLibrary === lib.id
                  ? 'bg-blue-500 text-white shadow-md'
                  : 'text-gray-600 hover:bg-gray-100'
              }`}
            >
              {lib.name}
            </button>
          ))}
        </div>
      </div>

      {/* 차트 타입 선택 */}
      <div className="flex justify-center mb-8 flex-wrap">
        <div className="bg-white rounded-lg p-1 shadow-md flex flex-wrap justify-center">
          {[
            { id: 'radar', name: '레이더 차트' },
            { id: 'bar', name: '바 차트' },
            { id: 'groupedBar', name: '그룹형 바 차트' },
            { id: 'line', name: '라인 차트' },
            { id: 'dotPlot', name: '닷 플롯' },
            { id: 'sparkline', name: '스파크라인' },
            { id: 'gauge', name: '게이지' },
          ].map(chart => (
            <button
              key={chart.id}
              onClick={() => setActiveChart(chart.id)}
              className={`px-4 py-2 rounded-md transition-all m-1 ${
                activeChart === chart.id
                  ? 'bg-green-500 text-white shadow-md'
                  : 'text-gray-600 hover:bg-gray-100'
              }`}
            >
              {chart.name}
            </button>
          ))}
        </div>
      </div>

      {/* 라이브러리 정보 */}
      <div className="bg-white rounded-lg shadow-lg p-4 mb-6">
        {activeLibrary === 'recharts' && (
          <div>
            <h3 className="font-semibold text-lg mb-2">Recharts 특징</h3>
            <p className="text-gray-600">✅ React 컴포넌트 방식으로 직관적 / ✅ 반응형 기본 지원 / ⚠️ 커스터마이징 제한적</p>
          </div>
        )}
        {activeLibrary === 'd3' && (
          <div>
            <h3 className="font-semibold text-lg mb-2">D3.js 특징</h3>
            <p className="text-gray-600">✅ 완전한 커스터마이징 가능 / ✅ 복잡한 시각화 구현 / ⚠️ 학습 곡선 높음</p>
          </div>
        )}
      </div>

      {/* 차트 렌더링 */}
      <div className="bg-white rounded-lg shadow-lg p-6">
        <h2 className="text-xl font-semibold mb-4 text-center">
          {activeLibrary === 'recharts' ? 'Recharts' : 'D3.js'} -
          {activeChart === 'radar' ? ' 레이더 차트' :
            activeChart === 'bar' ? ' 바 차트' :
              activeChart === 'groupedBar' ? ' 그룹형 바 차트' :
                activeChart === 'line' ? ' 라인 차트' :
                  activeChart === 'dotPlot' ? ' 닷 플롯' :
                    activeChart === 'sparkline' ? ' 스파크라인' : ' 게이지'}
        </h2>

        <div className="flex justify-center">
          {/* Recharts */}
          {activeLibrary === 'recharts' && (
            <div className="w-full">
              {activeChart === 'radar' && (
                <div className="h-96">
                  <ResponsiveContainer width="100%" height="100%">
                    <RadarChart data={securityData}>
                      <PolarGrid />
                      <PolarAngleAxis dataKey="subject" />
                      <PolarRadiusAxis domain={[0, 100]} />
                      <Radar
                        name="보안 점수"
                        dataKey="score"
                        stroke="#3B82F6"
                        fill="#3B82F6"
                        fillOpacity={0.3}
                        strokeWidth={2}
                      />
                    </RadarChart>
                  </ResponsiveContainer>
                </div>
              )}
              {activeChart === 'bar' && (
                <div className="h-80">
                  <ResponsiveContainer width="100%" height="100%">
                    <BarChart data={securityData}>
                      <CartesianGrid strokeDasharray="3 3" />
                      <XAxis dataKey="subject" angle={-45} textAnchor="end" height={100} />
                      <YAxis domain={[0, 100]} />
                      <Tooltip />
                      <Bar dataKey="score" fill="#3B82F6" />
                    </BarChart>
                  </ResponsiveContainer>
                </div>
              )}
              {activeChart === 'groupedBar' && (
                <div className="h-80">
                  <ResponsiveContainer width="100%" height="100%">
                    <BarChart data={salesData}>
                      <CartesianGrid strokeDasharray="3 3" />
                      <XAxis dataKey="category" />
                      <YAxis />
                      <Tooltip />
                      <Legend />
                      <Bar dataKey="product1" fill="#8884d8" />
                      <Bar dataKey="product2" fill="#82ca9d" />
                      <Bar dataKey="product3" fill="#ffc658" />
                    </BarChart>
                  </ResponsiveContainer>
                </div>
              )}
              {activeChart === 'line' && (
                <div className="h-80">
                  <ResponsiveContainer width="100%" height="100%">
                    <LineChart data={lineChartData}>
                      <CartesianGrid strokeDasharray="3 3" />
                      <XAxis dataKey="x" />
                      <YAxis />
                      <Tooltip />
                      <Legend />
                      <Line type="monotone" dataKey="y" stroke="#3B82F6" activeDot={{ r: 8 }} />
                    </LineChart>
                  </ResponsiveContainer>
                </div>
              )}
              {activeChart === 'dotPlot' && (
                <div className="h-80">
                  <ResponsiveContainer width="100%" height="100%">
                    <ScatterChart margin={{ top: 20, right: 20, bottom: 20, left: 20 }}>
                      <CartesianGrid />
                      <XAxis type="category" dataKey="id" name="Item" />
                      <YAxis type="number" dataKey="value" name="Value" domain={[0, 100]} />
                      <Tooltip cursor={{ strokeDasharray: '3 3' }} />
                      <Legend />
                      <Scatter name="Value" data={dotPlotData} fill="#3B82F6" />
                    </ScatterChart>
                  </ResponsiveContainer>
                </div>
              )}
              {activeChart === 'sparkline' && (
                <div className="h-24 w-48 mx-auto">
                  <ResponsiveContainer width="100%" height="100%">
                    <LineChart data={sparklineData.map((d, i) => ({ x: i, y: d }))} margin={{ top: 5, right: 5, bottom: 5, left: 5 }}>
                      <Line type="monotone" dataKey="y" stroke="#3B82F6" dot={false} strokeWidth={1} />
                      <Tooltip />
                    </LineChart>
                  </ResponsiveContainer>
                </div>
              )}
              {activeChart === 'gauge' && (
                <div className="flex justify-center">
                  <div className="text-center">
                    <div className="w-40 h-40 rounded-full bg-gray-200 flex items-center justify-center relative overflow-hidden">
                      <div
                        className="absolute bottom-0 left-0 w-full bg-blue-500 transition-all duration-1000"
                        style={{ height: `${overallScore}%` }}
                      />
                      <div className="relative z-10 text-center">
                        <div className="text-3xl font-bold text-white">{overallScore}</div>
                        <div className="text-sm text-white">점수</div>
                      </div>
                    </div>
                  </div>
                </div>
              )}
            </div>
          )}

          {/* D3.js */}
          {activeLibrary === 'd3' && (
            <div className="flex justify-center flex-col items-center w-full min-h-[300px]">
              {activeChart === 'radar' && <svg ref={d3RadarRef}></svg>}
              {activeChart === 'bar' && <svg ref={d3BarRef}></svg>}
              {activeChart === 'gauge' && <svg ref={d3GaugeRef}></svg>}
              {activeChart === 'groupedBar' && <svg ref={d3GroupedBarRef}></svg>}
              {activeChart === 'line' && <svg ref={d3LineRef}></svg>}
              {activeChart === 'dotPlot' && <svg ref={d3DotPlotRef}></svg>}
              {activeChart === 'sparkline' && <svg ref={d3SparklineRef}></svg>}
            </div>
          )}
        </div>

        {/* 코드 예시 */}
        <div className="mt-8 bg-gray-100 rounded-lg p-4">
          <h4 className="font-semibold mb-2">코드 복잡도 비교:</h4>
          {activeLibrary === 'recharts' && (
            <div className="text-sm text-gray-700">
              <strong>Recharts:</strong> 선언적, React 친화적 JSX 문법. 일반적인 차트 구현은 간결함. (코드 줄 수: ~10-30줄)
            </div>
          )}
          {activeLibrary === 'd3' && (
            <div className="text-sm text-gray-700">
              <strong>D3.js:</strong> 명령형, 데이터 기반 문서 조작. 세밀한 제어 가능하지만 모든 요소 직접 코딩 (코드 줄 수: ~50-200줄).
              *복잡한 차트일수록 D3의 코드가 기하급수적으로 늘어납니다.*
            </div>
          )}
        </div>
      </div>
    </div>
  );
};








const NivoVictorySimulation = () => {
  const [activeLibrary, setActiveLibrary] = useState('nivo-style');
  const [activeChart, setActiveChart] = useState('radar');

  // 샘플 데이터
  const securityData = [
    { subject: '네트워크 보안', score: 85, fullMark: 100 },
    { subject: '데이터 보안', score: 72, fullMark: 100 },
    { subject: '접근 제어', score: 90, fullMark: 100 },
    { subject: '물리적 보안', score: 65, fullMark: 100 },
    { subject: '인적 보안', score: 78, fullMark: 100 },
    { subject: '사고 대응', score: 55, fullMark: 100 }
  ];

  const salesData = [
    { category: 'A', product1: 150, product2: 120, product3: 80 },
    { category: 'B', product1: 180, product2: 140, product3: 95 },
    { category: 'C', product1: 200, product2: 160, product3: 110 },
    { category: 'D', product1: 220, product2: 180, product3: 125 },
    { category: 'E', product1: 250, product2: 200, product3: 140 },
  ];

  const lineChartData = [30, 45, 40, 55, 50, 65];
  const sparklineData = [10, 25, 18, 30, 22, 15, 28, 35, 20, 27];

  const overallScore = Math.round(securityData.reduce((sum, item) => sum + item.score, 0) / securityData.length);

  // Nivo 스타일 레이더 차트 시뮬레이션
  const NivoRadarChart = () => {
    const size = 300;
    const center = size / 2;
    const radius = 100;
    const levels = 5;

    const angleStep = (2 * Math.PI) / securityData.length;

    return (
      <div className="relative w-80 h-80 mx-auto">
        <svg width={size} height={size} className="absolute inset-0">
          {/* 격자 원들 */}
          {Array.from({ length: levels }, (_, i) => (
            <circle
              key={i}
              cx={center}
              cy={center}
              r={(radius * (i + 1)) / levels}
              fill="none"
              stroke="#e0e0e0"
              strokeWidth="1"
              className="animate-pulse"
            />
          ))}
          
          {/* 축 선들 */}
          {securityData.map((_, i) => {
            const angle = i * angleStep - Math.PI / 2;
            const x = center + Math.cos(angle) * radius;
            const y = center + Math.sin(angle) * radius;
            return (
              <line
                key={i}
                x1={center}
                y1={center}
                x2={x}
                y2={y}
                stroke="#e0e0e0"
                strokeWidth="1"
              />
            );
          })}

          {/* 데이터 영역 */}
          <polygon
            points={securityData.map((d, i) => {
              const angle = i * angleStep - Math.PI / 2;
              const distance = (radius * d.score) / 100;
              const x = center + Math.cos(angle) * distance;
              const y = center + Math.sin(angle) * distance;
              return `${x},${y}`;
            }).join(' ')}
            fill="rgba(59, 130, 246, 0.3)"
            stroke="#3B82F6"
            strokeWidth="2"
            className="animate-pulse"
          />

          {/* 데이터 포인트 */}
          {securityData.map((d, i) => {
            const angle = i * angleStep - Math.PI / 2;
            const distance = (radius * d.score) / 100;
            const x = center + Math.cos(angle) * distance;
            const y = center + Math.sin(angle) * distance;
            return (
              <circle
                key={i}
                cx={x}
                cy={y}
                r="4"
                fill="#3B82F6"
                className="hover:r-6 transition-all duration-300 cursor-pointer"
              />
            );
          })}
        </svg>

        {/* 레이블들 */}
        {securityData.map((d, i) => {
          const angle = i * angleStep - Math.PI / 2;
          const x = center + Math.cos(angle) * (radius + 30);
          const y = center + Math.sin(angle) * (radius + 30);
          return (
            <div
              key={i}
              className="absolute text-xs font-medium text-gray-700 transform -translate-x-1/2 -translate-y-1/2"
              style={{ left: x, top: y }}
            >
              {d.subject}
            </div>
          );
        })}

        {/* Nivo 스타일 애니메이션 인디케이터 */}
        <div className="absolute top-2 right-2 text-xs text-blue-500 font-bold animate-bounce">
          ✨ Nivo Style
        </div>
      </div>
    );
  };

  // Victory 스타일 레이더 차트 시뮬레이션
  const VictoryRadarChart = () => {
    const size = 300;
    const center = size / 2;
    const radius = 100;
    const angleStep = (2 * Math.PI) / securityData.length;

    return (
      <div className="relative w-80 h-80 mx-auto">
        <svg width={size} height={size} className="absolute inset-0">
          {/* Victory 스타일 배경 */}
          <rect width={size} height={size} fill="#fafafa" rx="8" />
          
          {/* 격자 */}
          {Array.from({ length: 5 }, (_, i) => (
            <circle
              key={i}
              cx={center}
              cy={center}
              r={(radius * (i + 1)) / 5}
              fill="none"
              stroke="#d0d0d0"
              strokeWidth="1"
            />
          ))}

          {/* 축 */}
          {securityData.map((_, i) => {
            const angle = i * angleStep - Math.PI / 2;
            const x = center + Math.cos(angle) * radius;
            const y = center + Math.sin(angle) * radius;
            return (
              <line
                key={i}
                x1={center}
                y1={center}
                x2={x}
                y2={y}
                stroke="#d0d0d0"
                strokeWidth="1"
              />
            );
          })}

          {/* Victory 스타일 데이터 영역 */}
          <polygon
            points={securityData.map((d, i) => {
              const angle = i * angleStep - Math.PI / 2;
              const distance = (radius * d.score) / 100;
              const x = center + Math.cos(angle) * distance;
              const y = center + Math.sin(angle) * distance;
              return `${x},${y}`;
            }).join(' ')}
            fill="rgba(255, 99, 132, 0.4)"
            stroke="#FF6384"
            strokeWidth="3"
          />

          {/* Victory 스타일 포인트 */}
          {securityData.map((d, i) => {
            const angle = i * angleStep - Math.PI / 2;
            const distance = (radius * d.score) / 100;
            const x = center + Math.cos(angle) * distance;
            const y = center + Math.sin(angle) * distance;
            return (
              <circle
                key={i}
                cx={x}
                cy={y}
                r="5"
                fill="#FF6384"
                stroke="white"
                strokeWidth="2"
              />
            );
          })}
        </svg>

        {/* Victory 스타일 레이블 */}
        {securityData.map((d, i) => {
          const angle = i * angleStep - Math.PI / 2;
          const x = center + Math.cos(angle) * (radius + 35);
          const y = center + Math.sin(angle) * (radius + 35);
          return (
            <div
              key={i}
              className="absolute text-xs font-semibold text-gray-800 transform -translate-x-1/2 -translate-y-1/2"
              style={{ left: x, top: y }}
            >
              {d.subject}
            </div>
          );
        })}

        <div className="absolute top-2 right-2 text-xs text-pink-500 font-bold">
          🏆 Victory Style
        </div>
      </div>
    );
  };

  // Nivo 스타일 바 차트
  const NivoBarChart = () => (
    <div className="w-full h-80 bg-gradient-to-br from-blue-50 to-indigo-50 rounded-lg p-4 relative overflow-hidden">
      <div className="flex items-end justify-around h-full">
        {securityData.map((d, i) => (
          <div key={i} className="flex flex-col items-center">
            <div
              className="bg-gradient-to-t from-blue-600 to-blue-400 rounded-t-md transition-all duration-1000 hover:from-blue-700 hover:to-blue-500 hover:scale-105 cursor-pointer shadow-lg"
              style={{ 
                height: `${(d.score / 100) * 200}px`, 
                width: '40px',
                animationDelay: `${i * 0.1}s`
              }}
            />
            <div className="text-xs mt-2 font-medium text-gray-700 text-center transform rotate-45 origin-bottom-left">
              {d.subject}
            </div>
            <div className="text-xs text-blue-600 font-bold mt-1">{d.score}</div>
          </div>
        ))}
      </div>
      <div className="absolute top-2 right-2 text-xs text-blue-500 font-bold animate-pulse">
        ✨ Nivo Style
      </div>
    </div>
  );

  // Victory 스타일 바 차트
  const VictoryBarChart = () => (
    <div className="w-full h-80 bg-gray-50 rounded-lg p-4 relative">
      <div className="flex items-end justify-around h-full">
        {securityData.map((d, i) => (
          <div key={i} className="flex flex-col items-center">
            <div
              className="bg-pink-500 rounded-sm transition-all duration-500 hover:bg-pink-600 cursor-pointer"
              style={{ 
                height: `${(d.score / 100) * 200}px`, 
                width: '35px',
                animationDelay: `${i * 0.15}s`
              }}
            />
            <div className="text-xs mt-2 font-semibold text-gray-800 text-center">
              {d.subject.split(' ')[0]}
            </div>
            <div className="text-xs text-pink-600 font-bold">{d.score}</div>
          </div>
        ))}
      </div>
      <div className="absolute top-2 right-2 text-xs text-pink-500 font-bold">
        🏆 Victory Style
      </div>
    </div>
  );

  // Nivo 스타일 라인 차트
  const NivoLineChart = () => (
    <div className="w-full h-80 bg-gradient-to-br from-purple-50 to-blue-50 rounded-lg p-6 relative">
      <svg width="100%" height="100%" className="overflow-visible">
        <defs>
          <linearGradient id="lineGradient" x1="0%" y1="0%" x2="100%" y2="0%">
            <stop offset="0%" stopColor="#8B5CF6" />
            <stop offset="100%" stopColor="#3B82F6" />
          </linearGradient>
        </defs>
        
        {/* 격자 */}
        {Array.from({ length: 6 }, (_, i) => (
          <line
            key={i}
            x1={`${(i * 100) / 5}%`}
            y1="0%"
            x2={`${(i * 100) / 5}%`}
            y2="100%"
            stroke="#e0e0e0"
            strokeWidth="1"
            strokeDasharray="2,2"
          />
        ))}
        
        <polyline
          points={lineChartData.map((d, i) => 
            `${(i / (lineChartData.length - 1)) * 100}%,${100 - (d / Math.max(...lineChartData)) * 80}%`
          ).join(' ')}
          fill="none"
          stroke="url(#lineGradient)"
          strokeWidth="3"
          className="drop-shadow-sm"
        />
        
        {/* 포인트 */}
        {lineChartData.map((d, i) => (
          <circle
            key={i}
            cx={`${(i / (lineChartData.length - 1)) * 100}%`}
            cy={`${100 - (d / Math.max(...lineChartData)) * 80}%`}
            r="5"
            fill="white"
            stroke="#3B82F6"
            strokeWidth="3"
            className="hover:r-7 transition-all duration-300 cursor-pointer drop-shadow-md"
          />
        ))}
      </svg>
      <div className="absolute top-2 right-2 text-xs text-purple-500 font-bold animate-pulse">
        ✨ Nivo Style
      </div>
    </div>
  );

  // Victory 스타일 라인 차트
  const VictoryLineChart = () => (
    <div className="w-full h-80 bg-white border-2 border-gray-200 rounded-lg p-6 relative">
      <svg width="100%" height="100%">
        {/* Victory 스타일 축 */}
        <line x1="0%" y1="100%" x2="100%" y2="100%" stroke="#333" strokeWidth="2" />
        <line x1="0%" y1="0%" x2="0%" y2="100%" stroke="#333" strokeWidth="2" />
        
        <polyline
          points={lineChartData.map((d, i) => 
            `${(i / (lineChartData.length - 1)) * 100}%,${100 - (d / Math.max(...lineChartData)) * 80}%`
          ).join(' ')}
          fill="none"
          stroke="#FF6384"
          strokeWidth="4"
        />
        
        {/* Victory 스타일 포인트 */}
        {lineChartData.map((d, i) => (
          <circle
            key={i}
            cx={`${(i / (lineChartData.length - 1)) * 100}%`}
            cy={`${100 - (d / Math.max(...lineChartData)) * 80}%`}
            r="6"
            fill="#FF6384"
            stroke="white"
            strokeWidth="2"
          />
        ))}
      </svg>
      <div className="absolute top-2 right-2 text-xs text-pink-500 font-bold">
        🏆 Victory Style
      </div>
    </div>
  );

  // 게이지 차트들
  const NivoGaugeChart = () => (
    <div className="flex justify-center">
      <div className="relative w-48 h-48">
        <svg width="192" height="192" className="absolute inset-0">
          <defs>
            <linearGradient id="gaugeGradient" x1="0%" y1="0%" x2="100%" y2="0%">
              <stop offset="0%" stopColor="#10B981" />
              <stop offset="50%" stopColor="#3B82F6" />
              <stop offset="100%" stopColor="#8B5CF6" />
            </linearGradient>
          </defs>
          
          {/* 배경 호 */}
          <path
            d="M 24 96 A 72 72 0 0 1 168 96"
            fill="none"
            stroke="#e5e7eb"
            strokeWidth="12"
          />
          
          {/* 점수 호 */}
          <path
            d={`M 24 96 A 72 72 0 0 1 ${96 + Math.cos(Math.PI - (Math.PI * overallScore / 100)) * 72} ${96 - Math.sin(Math.PI - (Math.PI * overallScore / 100)) * 72}`}
            fill="none"
            stroke="url(#gaugeGradient)"
            strokeWidth="12"
            strokeLinecap="round"
            className="drop-shadow-sm"
          />
        </svg>
        
        <div className="absolute inset-0 flex flex-col items-center justify-center">
          <div className="text-4xl font-bold text-gray-800 animate-pulse">{overallScore}</div>
          <div className="text-sm text-gray-600">점수</div>
        </div>
        
        <div className="absolute top-2 right-2 text-xs text-blue-500 font-bold">
          ✨ Nivo
        </div>
      </div>
    </div>
  );

  const VictoryGaugeChart = () => (
    <div className="flex justify-center">
      <div className="relative w-48 h-48 bg-gray-50 rounded-lg">
        <svg width="192" height="192" className="absolute inset-0">
          {/* Victory 스타일 배경 */}
          <path
            d="M 24 96 A 72 72 0 0 1 168 96"
            fill="none"
            stroke="#d1d5db"
            strokeWidth="8"
          />
          
          {/* Victory 스타일 점수 호 */}
          <path
            d={`M 24 96 A 72 72 0 0 1 ${96 + Math.cos(Math.PI - (Math.PI * overallScore / 100)) * 72} ${96 - Math.sin(Math.PI - (Math.PI * overallScore / 100)) * 72}`}
            fill="none"
            stroke="#FF6384"
            strokeWidth="8"
            strokeLinecap="round"
          />
        </svg>
        
        <div className="absolute inset-0 flex flex-col items-center justify-center">
          <div className="text-3xl font-bold text-gray-800">{overallScore}</div>
          <div className="text-sm text-gray-600">점수</div>
        </div>
        
        <div className="absolute top-2 right-2 text-xs text-pink-500 font-bold">
          🏆 Victory
        </div>
      </div>
    </div>
  );

  return (
    <div className="p-6 max-w-6xl mx-auto bg-gray-50 min-h-screen">
      <h1 className="text-3xl font-bold text-center mb-8 text-gray-800">
        Nivo & Victory 스타일 시뮬레이션
      </h1>
      
      <div className="mb-6 text-center">
        <div className="bg-yellow-100 border border-yellow-400 text-yellow-700 px-4 py-3 rounded mb-4">
          <strong>참고:</strong> 이는 실제 Nivo/Victory 라이브러리가 아닌 CSS/SVG를 이용한 스타일 시뮬레이션입니다.
        </div>
      </div>

      {/* 라이브러리 선택 */}
      <div className="flex justify-center mb-6">
        <div className="bg-white rounded-lg p-1 shadow-md">
          {[
            { id: 'nivo-style', name: '✨ Nivo Style' },
            { id: 'victory-style', name: '🏆 Victory Style' },
          ].map(lib => (
            <button
              key={lib.id}
              onClick={() => setActiveLibrary(lib.id)}
              className={`px-6 py-2 rounded-md transition-all ${
                activeLibrary === lib.id
                  ? (lib.id === 'nivo-style' ? 'bg-blue-500 text-white shadow-md' : 'bg-pink-500 text-white shadow-md')
                  : 'text-gray-600 hover:bg-gray-100'
              }`}
            >
              {lib.name}
            </button>
          ))}
        </div>
      </div>

      {/* 차트 타입 선택 */}
      <div className="flex justify-center mb-8 flex-wrap">
        <div className="bg-white rounded-lg p-1 shadow-md flex flex-wrap justify-center">
          {[
            { id: 'radar', name: '레이더 차트' },
            { id: 'bar', name: '바 차트' },
            { id: 'line', name: '라인 차트' },
            { id: 'gauge', name: '게이지' },
          ].map(chart => (
            <button
              key={chart.id}
              onClick={() => setActiveChart(chart.id)}
              className={`px-4 py-2 rounded-md transition-all m-1 ${
                activeChart === chart.id
                  ? 'bg-green-500 text-white shadow-md'
                  : 'text-gray-600 hover:bg-gray-100'
              }`}
            >
              {chart.name}
            </button>
          ))}
        </div>
      </div>

      {/* 라이브러리 특징 설명 */}
      <div className="bg-white rounded-lg shadow-lg p-4 mb-6">
        {activeLibrary === 'nivo-style' && (
          <div>
            <h3 className="font-semibold text-lg mb-2 text-blue-600">✨ Nivo 스타일 특징</h3>
            <p className="text-gray-600">
              🎨 <strong>풍부한 애니메이션:</strong> 부드럽고 세련된 트랜지션 효과<br/>
              🌈 <strong>아름다운 그래디언트:</strong> 현대적인 색상 조합과 그래디언트<br/>
              ⚡ <strong>인터랙티브:</strong> 호버 효과와 반응형 디자인<br/>
              📱 <strong>반응형:</strong> 다양한 화면 크기에 최적화
            </p>
          </div>
        )}
        {activeLibrary === 'victory-style' && (
          <div>
            <h3 className="font-semibold text-lg mb-2 text-pink-600">🏆 Victory 스타일 특징</h3>
            <p className="text-gray-600">
              🎯 <strong>깔끔한 디자인:</strong> 명확하고 읽기 쉬운 시각화<br/>
              🔧 <strong>모듈형 구조:</strong> 컴포넌트 조합으로 유연한 구성<br/>
              📊 <strong>전문적 외관:</strong> 비즈니스 환경에 적합한 스타일<br/>
              🚀 <strong>성능 최적화:</strong> 빠른 렌더링과 부드러운 애니메이션
            </p>
          </div>
        )}
      </div>

      {/* 차트 렌더링 */}
      <div className="bg-white rounded-lg shadow-lg p-6">
        <h2 className="text-xl font-semibold mb-4 text-center">
          {activeLibrary === 'nivo-style' ? '✨ Nivo 스타일' : '🏆 Victory 스타일'} -
          {activeChart === 'radar' ? ' 레이더 차트' :
            activeChart === 'bar' ? ' 바 차트' :
              activeChart === 'line' ? ' 라인 차트' : ' 게이지'}
        </h2>

        <div className="flex justify-center">
          {activeLibrary === 'nivo-style' && (
            <div className="w-full">
              {activeChart === 'radar' && <NivoRadarChart />}
              {activeChart === 'bar' && <NivoBarChart />}
              {activeChart === 'line' && <NivoLineChart />}
              {activeChart === 'gauge' && <NivoGaugeChart />}
            </div>
          )}
          
          {activeLibrary === 'victory-style' && (
            <div className="w-full">
              {activeChart === 'radar' && <VictoryRadarChart />}
              {activeChart === 'bar' && <VictoryBarChart />}
              {activeChart === 'line' && <VictoryLineChart />}
              {activeChart === 'gauge' && <VictoryGaugeChart />}
            </div>
          )}
        </div>

        {/* 스타일 비교 */}
        <div className="mt-8 bg-gray-100 rounded-lg p-4">
          <h4 className="font-semibold mb-2">스타일 특징:</h4>
          {activeLibrary === 'nivo-style' && (
            <div className="text-sm text-gray-700">
              <strong>✨ Nivo 스타일:</strong> 현대적이고 세련된 디자인, 부드러운 애니메이션, 그래디언트와 그림자 효과 활용.
              데이터 시각화에 감성적 요소를 더해 사용자 경험을 향상시킵니다.
            </div>
          )}
          {activeLibrary === 'victory-style' && (
            <div className="text-sm text-gray-700">
              <strong>🏆 Victory 스타일:</strong> 깔끔하고 전문적인 디자인, 명확한 데이터 표현에 집중.
              비즈니스 환경과 대시보드에 적합한 직관적인 스타일을 제공합니다.
            </div>
          )}
        </div>
      </div>
    </div>
  );
};




export default LibraryComparisonDemo;
