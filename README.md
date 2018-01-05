# Optimal-trade-allocation
The power-percentile impact model - Suppose we want to sell a large number N shares of stock. Assuming the current price of stock is P0, the process of selling the stock will affect the price of the stock, with the result that the price will go down. Moreover, the trade will not be executed at once. We maybe able to sell k &lt; N number of shares at a time t, and the remaining N - k number of shares at time T - t. This algorithm determines optimal execution of the trade, by breaking the share allocation to n1, n2,,,,,,N over time periods t1, t2,....T, so as to maximize the revenue.
When N is large, the trade might take several days, during which period the price of the stock may evolve substantially, quite likely against our interest.   When all is said and done, instead of receiving a cash flow of P0N, we will instead receive P0N – M, for some M > 0.  The quantity is called the “market impact” of our trade – we want to minimize M [note: there are other, routine, smaller magnitude trading costs incurred when one trades.  These are smaller and unavoidable, and are not the subject of our discussion].
It seems prudent, therefore, when N is large, to space our trade into more easily digestible bites.  If we control the size of these partial trades, we may be able to minimize the market impact.  In order to quantify this statement precisely, we need two ingredients:

•	A precise numerical model of how market impact arises

•	An algorithm that minimizes market impact, given the model we have constructed

Some of the background for the models we will describe can be found in papers and on “folklore” ideas.  The algorithmic methodology we will use is based on Dynamic Programming; you can find good descriptions of this in many books on Mathematical Programming and on the web.
