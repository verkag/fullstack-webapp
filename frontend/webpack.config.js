const path = require("path");

module.exports = {
    entry: "./src/main.ts",
    module: {
        rules: [
            {
                test: /\.ts$/,
                use: "ts-loader",
                exclude: /node_modules/,
            },
            {
                test: /\.css$/,
                use: ["style-loader", "css-loader"],
            },
            {
                test: /\.(png|jpg|jpeg|gif|svg|ico)$/i,
                type: "assest/resource",
            },
        ],
    },
    resolve: {
        extensions: [".ts", ".js"],
    },
    output: {
        path: path.resolve(__dirname, "public/dist"),
        filename: "bundle.js",
    },
    mode: "development",
    devtool: "inline-source-map",
    devServer: {
        static: path.join(__dirname, "public"),
        port: 5555,
    },
    stats: {
        children: true,
        errorDetails: true,
    },
};
