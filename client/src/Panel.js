import React from "react";
import ReactDOM from "react-dom";

import "./components/App/App.css";

const App = require("../lib/js/src/components/App/App.bs").default;

ReactDOM.render(<App />, document.getElementById("root"));
